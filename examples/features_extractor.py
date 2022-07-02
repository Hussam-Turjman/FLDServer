# Author: Mohammed Hussam Al Turjman (hussam.turjman@gmail.com)
# Simple standalone script to extract features from videos in a given directory.

import abc
import base64
import csv
import logging
import multiprocessing as mp
import os
import sys
import time
from functools import partial
from typing import List, Tuple

import cv2 as cv
import numpy as np
import requests
import uuid

# Logger stuff
BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE = range(8)

# The background is set with 40 plus the number of the color,
# and the foreground with 30

# These are the sequences need to get colored output

RESET_SEQ = "\033[0m"
COLOR_SEQ = "\033[1;%dm"
BOLD_SEQ = "\033[1m"


def formatter_message(message, use_color=True):
    if use_color:
        message = message.replace("$RESET", RESET_SEQ).replace("$BOLD", BOLD_SEQ)
    else:
        message = message.replace("$RESET", "").replace("$BOLD", "")
    return message


COLORS = {
    'WARNING': YELLOW,
    'INFO': BLUE,
    'DEBUG': GREEN,
    'CRITICAL': MAGENTA,
    'ERROR': RED
}


class ColoredFormatter(logging.Formatter):
    def __init__(self, msg, use_color=True):
        logging.Formatter.__init__(self, msg)
        self.use_color = use_color

    def format(self, record):
        levelname = record.levelname
        if self.use_color and levelname in COLORS:
            levelname_color = COLOR_SEQ % (30 + COLORS[levelname]) + levelname + RESET_SEQ
            record.levelname = levelname_color
        return logging.Formatter.format(self, record)


# Custom logger class with multiple destinations
class ColoredLogger(logging.Logger):
    FORMAT = "[%(asctime)s - %(levelname)-10s - %(name)s]  %(message)s ($BOLD%(filename)s$RESET:%(lineno)d)"
    COLOR_FORMAT = formatter_message(FORMAT, True)

    def __init__(self, name, level=logging.INFO):
        logging.Logger.__init__(self, name, level)

        # formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s - %(filename)s %(lineno)d')

        # fh = logging.FileHandler('debug.log')
        # fh.setLevel(level)
        # fh.setFormatter(formatter)
        # self.addHandler(fh)

        color_formatter = ColoredFormatter(self.COLOR_FORMAT)

        console = logging.StreamHandler()
        console.setFormatter(color_formatter)

        self.addHandler(console)


logging.setLoggerClass(ColoredLogger)

logger = ColoredLogger("StreamsClient")

# Current working directory
cwd = os.path.dirname(os.path.realpath(__file__))


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


class VideoStreamReaderCallback(abc.ABC):

    @abc.abstractmethod
    def on_frame(self, timestamp: float, frame: np.ndarray):
        """

        """


class VideoStreamReader(object):
    """
    Simple structure describing a video
    """
    # [(Timestamp, OpenCV Frame), ...]
    frames: List[Tuple[float, np.ndarray]]

    def __init__(self, video_path: str):
        """

        """
        self.path = video_path
        self.reader = None
        self.number_of_frames = None
        self.fps = None
        self.frame_width = None
        self.frame_height = None
        self.duration = None  # In seconds
        self.frames = []

    def load(self):
        """

        """
        if not os.path.exists(self.path):
            logger.error(f"Video path {self.path} does not exist")
            return False
        self.reader = cv.VideoCapture(self.path)
        if not self.reader.isOpened():
            logger.error(f"Failed to open the video located at : {self.path}")
            return False
        self.number_of_frames = self.reader.get(cv.CAP_PROP_FRAME_COUNT)
        assert self.number_of_frames.is_integer(), f"Invalid number of frames : {self.number_of_frames}"
        self.number_of_frames = int(self.number_of_frames)
        self.fps = self.reader.get(cv.CAP_PROP_FPS)
        self.frame_width = self.reader.get(cv.CAP_PROP_FRAME_WIDTH)
        self.frame_height = self.reader.get(cv.CAP_PROP_FRAME_HEIGHT)
        assert self.frame_height.is_integer(), f"Invalid frame height {self.frame_height}"
        assert self.frame_width.is_integer(), f"Invalid frame width {self.frame_width}"
        self.frame_height = int(self.frame_height)
        self.frame_width = int(self.frame_width)

        self.duration = self.number_of_frames / self.fps

        return True

    def next_frame(self):
        return self.reader.read()

    def reload(self):
        if self.reader is None:
            return False
        self.reader.release()

        return self.load()

    def duration_str(self):
        return time.strftime('%H:%M:%S', time.gmtime(self.duration))

    def use_frames(self, callback: VideoStreamReaderCallback):
        """

        """
        if not self.reload():
            return False

        timestamp = 0.0
        step = 1.0 / self.fps

        while True:
            ret, frame = self.next_frame()
            if not ret:
                return True
            callback.on_frame(timestamp, frame)
            timestamp += step

    def extract_frames(self):
        """

        """
        if not self.reload():
            return False

        self.frames.clear()
        timestamp = 0.0
        step = 1.0 / self.fps
        logger.info(f"Extracting frames from {self.path}")
        counter = 0
        while True:
            ret, frame = self.next_frame()
            if not ret:
                print(os.linesep)
                return len(self.frames) > 0
            self.frames.append((timestamp, frame))
            status = r"%10d  [%3.2f%%]" % (counter, counter * 100. / self.number_of_frames)
            status = status + chr(8) * (len(status) + 1)
            print(status, end="")
            timestamp += step
            counter += 1

    def show(self):
        """

        """
        if len(self.frames) == 0:
            return False

        logger.info(f"Showing : {os.linesep} {self}")
        for timestamp, frame in self.frames:
            cv.imshow('Frame', frame)
            if cv.waitKey(25) & 0xFF == ord('q'):
                return True

        return True

    def __str__(self):
        """

        """
        info = f"Frame: width = {self.frame_width}, height = {self.frame_height} {os.linesep}"
        info += f"FPS = {self.fps}, Number of frames = {self.number_of_frames} {os.linesep}"
        info += f"Duration = {self.duration} seconds {os.linesep}"
        info += f"Duration = {self.duration_str()}"
        return info


class VideoFile(object):
    reader: VideoStreamReader

    def __init__(self, absolute_path: str, sub_folder_path: str, parent_sub_path: str):
        """
        absolute_path : absolute path of the video file.
        sub_folder_path: Only the folder path starting from a
                         subdirectory to the directory where the file is located
        parent_sub_path: Sub path of the video file relative
                         to the main parent directory.

        """
        self.absolute_path = absolute_path
        self.file_name = os.path.basename(self.absolute_path)
        self.sub_folder_path = sub_folder_path
        self.last_directory = os.path.basename(os.path.normpath(self.sub_folder_path))
        self.parent_sub_path = parent_sub_path

        self.reader = None

    def __str__(self):
        """

        """
        info = f"File name : {self.file_name} {os.linesep}"
        info += f"Absolute path : {self.absolute_path} {os.linesep}"
        info += f"Sub folder path : {self.sub_folder_path} {os.linesep}"
        info += f"Parent sub path : {self.parent_sub_path} {os.linesep}"
        info += f"Last directory : {self.last_directory} {os.linesep}"
        return info + str(self.reader)

    def load(self):
        """

        """
        reader = VideoStreamReader(self.absolute_path)
        if not reader.load():
            return False
        self.reader = reader
        return True

    @classmethod
    def create(cls, absolute_path: str, sub_folder_path: str, parent_sub_path: str):
        return cls(absolute_path, sub_folder_path, parent_sub_path)


def read_videos_from_directory(folder: str, max_files: int = 0):
    """
    folder: absolute path of a folder that only contains video files
    max_files: maximum number of files to read.
    """
    video_files = []
    counter = 0
    for root, subdirs, files in os.walk(folder):

        for file in files:
            absolute_path = os.path.join(root, file)
            parent_sub_path = os.path.relpath(os.path.join(root, absolute_path), folder)
            sub_folder_path = os.path.dirname(parent_sub_path)
            if not absolute_path.endswith('.mp4'):
                continue
            video_file = VideoFile.create(absolute_path, sub_folder_path, parent_sub_path)

            video_files.append(video_file)
            if max_files > 0:
                counter += 1
                if counter >= max_files:
                    return video_files

    return video_files


# Print iterations progress
# https://stackoverflow.com/questions/3173320/text-progress-bar-in-the-console
def progress_bar(iteration, total, prefix='', suffix='', decimals=1, length=100, fill='█', print_end="\r"):
    """
    Call in a loop to create terminal progress bar
    @params:
        iteration   - Required  : current iteration (Int)
        total       - Required  : total iterations (Int)
        prefix      - Optional  : prefix string (Str)
        suffix      - Optional  : suffix string (Str)
        decimals    - Optional  : positive number of decimals in percent complete (Int)
        length      - Optional  : character length of bar (Int)
        fill        - Optional  : bar fill character (Str)
        printEnd    - Optional  : end character (e.g. "\r", "\r\n") (Str)
    """
    percent = ("{0:." + str(decimals) + "f}").format(100 * (iteration / float(total)))
    filledLength = int(length * iteration // total)
    bar = fill * filledLength + '-' * (length - filledLength)
    print(f'\r{prefix} |{bar}| {percent}% {suffix}', end=print_end)
    # Print New Line on Complete
    if iteration == total:
        print()


class ExtractionCallback(VideoStreamReaderCallback):
    def __init__(self, video_file: VideoFile, view_features: bool):
        self.video_file = video_file
        self.frame_id = -1
        self.stopped = False
        self.view_features = view_features
        self.with_vis_image = view_features
        self.features = []
        self.csv_absolute_path = f"{os.path.splitext(self.video_file.absolute_path)[0]}.csv"
        self.session_id = str(uuid.uuid4())

    def export_to_csv(self):
        """

        """
        if len(self.features) == 0:
            logger.error("No features to export")
            return
        print(os.linesep)
        with open(self.csv_absolute_path, "w+") as f:
            writer = csv.writer(f, delimiter=",")
            first_frame = self.features[0]
            keys = sorted(list(first_frame.keys()))
            writer.writerow(keys)
            for i in range(len(self.features)):
                values = []
                for key in keys:
                    values.append(self.features[i][key])
                writer.writerow(values)
                progress_bar(i, len(self.features),
                             prefix='Exporting To CSV Progress:', suffix='Complete',
                             length=20)
            f.close()

    def on_frame(self, timestamp: float, frame: np.ndarray):
        if self.stopped:
            return
        video_frame = VideoFrame(frame)

        image_size, base64_str = video_frame.to_base64(".jpg")

        self.frame_id += 1
        progress_bar(self.frame_id, self.video_file.reader.number_of_frames,
                     prefix='Feature Extraction Progress:', suffix='Complete',
                     length=20)

        try:
            response = requests.post("http://127.0.0.1:34568/",
                                     json={"image": base64_str,
                                           "size": image_size,
                                           "width": self.video_file.reader.frame_width,
                                           "height": self.video_file.reader.frame_height,
                                           "session_id": self.session_id,
                                           "fps": self.video_file.reader.fps,
                                           "with_vis_image": self.with_vis_image})
            features = None
            if response.status_code == 200:
                features = response.json()
            elif response.status_code == 500:
                features = None
            else:
                logger.error(f"Unexpected status code = {response.status_code}")

            if features is not None:

                if self.with_vis_image:
                    vis_video_frame = VideoFrame.from_base64(features["vis_image"], cv.IMREAD_COLOR)

                    cv.imshow('Frame', vis_video_frame.pixels)

                    if cv.waitKey(25) & 0xFF == ord('q'):
                        self.stopped = True
                        return

                    features.pop("vis_image")
                    features.pop("vis_image_width")
                    features.pop("vis_image_height")

                features["frame"] = self.frame_id
                features["timestamp"] = timestamp
                self.features.append(features)
        except Exception as e:
            logger.error(f"Failed to send request to the FLDServer. Error :  {e}")
            sys.exit(0)


def extract_features(video_file: VideoFile, view_features: bool):
    """

    """

    assert video_file.load(), f"Failed to load the video file {video_file}"

    fake_callback = ExtractionCallback(video_file, view_features)
    logger.info(f"\n{video_file}")
    video_file.reader.use_frames(fake_callback)
    fake_callback.export_to_csv()
    print(os.linesep)
    logger.info("--------------------")


def extract_frames_from_videos(video_files: List[VideoFile], view_features: bool, cores: int):
    """

    """
    if len(video_files) == 0:
        logger.error("No video files in the given directory")
        sys.exit(0)

    pool = mp.Pool(cores)
    callback = partial(extract_features, view_features=view_features)
    pool.map(callback, video_files)
    pool.close()
    pool.join()


def run(directory: str, view_features=True, cores=1):
    """

    """
    logger.info("Starting ..")

    videos_folder_path = os.path.join(cwd, directory)
    if not os.path.exists(videos_folder_path):
        logger.error(f"Videos path {videos_folder_path} does not exist. ")
        sys.exit(0)

    video_files = read_videos_from_directory(videos_folder_path)
    extract_frames_from_videos(video_files, view_features, cores)


if __name__ == "__main__":
    # FIXME Currently the FLDServer does not work with multiple clients
    #  so keep cores=1 .
    run(directory="Videos", view_features=True, cores=5)
