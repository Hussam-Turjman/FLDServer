# Simple camera demo for using the FLDServer.
# 1 - read video frame from the video capture device
# 2 - encode the frame and convert it to base64 string
# 3 - send it to the FLDServer
# 4 - read the response and extract the features

import base64
import os
import time
from typing import Tuple

import cv2 as cv
import numpy as np
import requests
import uuid


class VideoCapture(object):

    def __init__(self):
        self.cap = cv.VideoCapture(0)
        self._is_open = False
        self.print_info()

    def print_info(self):
        """

        """
        info = f"VideoCap info: {os.linesep}"
        info += f"Frame :  width = {self.frame_width()}, " \
                f"height = {self.frame_height()}, channel = {self.channel()}, "
        info += f"FPS = {self.fps()}, "
        info += f"Format = {self.format()}, Fourcc = {self.fourcc()}"
        print(info)

    def set_width(self, width: int):
        return self.cap.set(cv.CAP_PROP_FRAME_WIDTH, width)

    def set_height(self, height: int):
        return self.cap.set(cv.CAP_PROP_FRAME_HEIGHT, height)

    def frame_width(self):
        width = self.cap.get(cv.CAP_PROP_FRAME_WIDTH)
        assert width.is_integer()
        return int(width)

    def frame_height(self):
        height = self.cap.get(cv.CAP_PROP_FRAME_HEIGHT)
        assert height.is_integer()
        return int(height)

    def fourcc(self):
        return self.cap.get(cv.CAP_PROP_FOURCC)

    def fps(self):
        return self.cap.get(cv.CAP_PROP_FPS)

    def channel(self):
        return self.cap.get(cv.CAP_PROP_CHANNEL)

    def format(self):
        return self.cap.get(cv.CAP_PROP_FORMAT)

    def is_open(self):
        return self._is_open

    def open(self):
        self._is_open = self.cap.open(0)
        return self.is_open()

    def read(self):
        if not self.is_open():
            return None
        return self.cap.read()

    def close(self):
        """

        :return:
        """
        if not self.is_open():
            return False
        self._is_open = self.cap.release()
        return not self._is_open


class VideoFrame(object):
    pixels: np.ndarray
    __features: dict

    def __init__(self, pixels: np.ndarray):
        self.pixels = pixels
        self.__features = {}

    @property
    def features(self) -> dict:
        return self.__features

    def set_features(self, features: dict):
        self.__features = features

    @classmethod
    def decode(cls, buff: np.ndarray, flags: int):
        """
        Convert a vector of bytes into image pixels.
        flags: color channels. For example : cv.IMREAD_COLOR
        """
        return cls(cv.imdecode(buff, flags))

    @classmethod
    def from_base64(cls, str_buffer: str, flags: int):
        """
        Convert base64-utf-8-string to image pixels.

        """
        s = base64.b64decode(str_buffer)
        buff = np.frombuffer(s, dtype='uint8')
        return VideoFrame.decode(buff, flags)

    def encode(self, ext: str = ".jpg") -> Tuple[bool, np.ndarray]:
        """
        Compress the current image pixels using the given extension.

        ext: File extension that defines the output format.
        return value: (success,bytes)
        """
        return cv.imencode(ext, self.pixels)

    def to_base64(self, ext: str = ".jpg") -> Tuple[int, str]:
        """
        Encode the current pixels and convert to base64 utf-8-string.
        return value: (encoded buffer size,utf-8-string)
        """
        ret, buff = self.encode(ext)
        assert ret, "Failed to encode current image pixels"
        return buff.shape[0], base64.b64encode(buff.data).decode("utf-8")


def run(server_url, save_video):
    print()
    print()
    print("Press q to exit ")
    print()
    print()
    session_id = str(uuid.uuid4())
    print(f"Session : {session_id}")
    with_vis_image = True
    cap = VideoCapture()

    if not cap.open():
        print("Cannot open camera")
        exit()
    # Define the codec and create VideoWriter object
    fourcc = cv.VideoWriter_fourcc(*'XVID')
    if save_video:
        video_out = cv.VideoWriter('output.avi', fourcc, 20.0, (cap.frame_width(), cap.frame_height()))
    else:
        video_out = None

    while True:
        # Capture frame-by-frame
        ret, frame = cap.read()
        # if frame is read correctly ret is True
        if not ret:
            print("Can't receive frame (stream end?). Exiting ...")
            break
        # Our operations on the frame come here
        flipped = cv.flip(frame, 1)
        # Send the frame to the FLDServer
        video_frame = VideoFrame(flipped)
        image_size, base64_str = video_frame.to_base64(".jpg")
        height, width, channels = video_frame.pixels.shape
        fps = cap.fps()
        vis_frame = None
        try:
            response = requests.post(server_url,
                                     json={"image": base64_str,
                                           "size": image_size,
                                           "width": width,
                                           "height": height,
                                           "session_id": session_id,
                                           "fps": fps,
                                           "with_vis_image":with_vis_image})

            features = None
            if response.status_code == 200:
                features = response.json()
            if features is not None:
                video_frame.set_features(features)
            else:
                print(f"Didn't receive any features .. {response}")
                exit(0)
            vis_image = video_frame.features["vis_image"] if with_vis_image else base64_str
            vis_frame = VideoFrame.from_base64(vis_image, cv.IMREAD_COLOR)

        except Exception as e:
            print(f"Failed to send frame to {server_url}: {e}")
            exit(0)

        # Display the resulting frame
        cv.imshow('FLDServer Camera Demo', vis_frame.pixels)

        if video_out is not None:
            video_out.write(vis_frame.pixels)

        if with_vis_image:
            # Remove visualization data
            video_frame.features.pop("vis_image")
            video_frame.features.pop("vis_image_width")
            video_frame.features.pop("vis_image_height")

        # Actual features received from the FLDServer
        confidence = video_frame.features["confidence"]

        frame_id = video_frame.features["frame"]
        timestamp = video_frame.features["timestamp"]
        print(
            f"Received {len(video_frame.features.keys())} features,"
            f" FrameID={frame_id}, Confidence={confidence}"
            f" Timestamp={timestamp}",

            end="\r")

        if cv.waitKey(1) == ord('q'):
            break
    # When everything done, release the capture
    assert cap.close()

    if video_out is not None:
        video_out.release()
    cv.destroyAllWindows()
    print()


if __name__ == "__main__":
    run(server_url="http://127.0.0.1:34568/", save_video=False)
