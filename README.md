# GUtilitiesAPI

[![Build status](https://ci.appveyor.com/api/projects/status/fhnddtpnb0o085b0?svg=true)](https://ci.appveyor.com/project/gu-on/gutilitiesapi)
[![Donate](https://img.shields.io/badge/Donate-Ko--fi-orange?logo=kofi&color=ff5f5f)](https://ko-fi.com/guonaudio)

Reworking of [GUtilities](https://github.com/gu-on/GUtilities/) where the C++ backend has been separated out into a public API that is callable via ReaScript. Additionally, Mac and Linux support is now provided. 

## Installation guide

Clone the repository:

    git clone https://github.com/gu-on/GUtilitiesAPI.git

GUtilitiesAPI is based on [ak5k's reaper-sdk-vscode template](https://github.com/ak5k/reaper-sdk-vscode), which includes instructions for getting set up on the various supported platforms. 

`gu_main.cpp` shouldn't be edited manually as it is automatically generated by `apigen.py`, which runs during the build process. It writes the code necessary to export the functions declared in `gu_api.hpp`. The `apigen.py` parses naively, expecting `//` comments followed by the function declaration (it also ignores #includes and blank lines). 

### Requirements:

- [CMake](https://cmake.org/) 3.15+
- C++17 compiler (MSVC on Windows)
- [Python](https://www.python.org/) 3.6+