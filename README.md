## Facial landmark detection server (FLDServer)
This project aims to provide a microservice for retrieving facial landmarks and features
using [OpenFace](https://github.com/TadasBaltrusaitis/OpenFace) in realtime via simple REST-API.


### Build
#### Supported compilers
- ``msvc``
- ``gcc``
#### Supported platforms
- ``x64``

In order to build the source code, you need to have at least CMake >=3.17.

- Following libs and tools are required on Linux:
    - ``` gcc->=10, g++>=10```
    - ```pkg-config ```
    - ```python-dev python-numpy libdc1394-22-dev libbrotli-dev ```
    
- Following tools are required on Windows:
   - ```visual studio build tools 2019``` [link](https://visualstudio.microsoft.com/downloads/) 
   
  
After installing all necessary libs and tools you can just call ``python build.py``. It will take while, until the
third_party libs are built.
On Windows you must call the ``build.py`` script from the Developer Command Prompt for VS to ensure ``msvc`` is used. Otherwise
you should know what you are doing.



### OpenFace Model

The necessary model for OpenFace can be downloaded from [here](https://drive.google.com/file/d/1Qc7oJJUfDXwt5e76ovTfdsoqa6oiR8JM/view?usp=sharing).

### REST-API

Currently, only the following methods are supported:

- POST with a json payload to ``http://127.0.0.1:34568/``

```json
  {
  "image": "base64 image",
  "size": "size in bytes",
  "width": "width of the image",
  "height": "height of the image",
  "frame_id": "frame_id",
  "timestamp": "timestamp"
}
```

### Examples
To see some demos go to the examples folder [here](./examples/README.md).

### Copyright
This project is distributed under the 3-Clause BSD [License](./LICENSE)

### Disclaimer

This project is still under development and may crash if used outside the above defined REST-API.

