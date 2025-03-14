# [YoAudio](../README.md)
[![Build Status](https://travis-ci.org/fuzzblob/YoAudio.svg?branch=master)](https://travis-ci.org/fuzzblob/YoAudio)  [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

- [The Editor Project](#editor)

## YoAudio Editor <a name="editor"></a>

The [YoAudio Editor](https://github.com/fuzzblob/YoAudioEditor) repository is a legacy project that was meant to be cloned into a folder adjacent to the YoAudio repository and built seperately. It since been merged into the [YoAudio](https://github.com/fuzzblob/YoAudio) repository under the `/editor/` folder (along with it's entire git commit history). Hence the original editor repository can be ignored.

Currently the engine project outputs it's library files to the same directory as where the editor project is looking for them. This might mean that the library needs to be built for the editor project to properly configure in CMake. It should be possible to not need the `.dll`/`.so` file to be searched for during the CMake project setup. On every build the editor ***should*** copy the `YoAudio-Engine.dll`/`.so` file to it's build folder.
