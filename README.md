![image: fox with headphones](docs/img/fox_small.png)

# [YoAudio](https://github.com/fuzzblob/YoAudio)
[![Build Status](https://travis-ci.org/fuzzblob/YoAudio.svg?branch=master)](https://travis-ci.org/fuzzblob/YoAudio)  [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

- [About](#about)
	- [Usage](docs/usage.md)
	- [The Editor](docs/editor.md)
	- [Roadmap](docs/roadmap.md)
- [Build Instructions](docs/build.md)
- [Acknowledgements](#acknowledge)

## About <a name="about"></a>

**WARNING! YoAudio is not production ready!**

YoAudio is a C++ audio engine meant as a playground for experimentation with game audio concepts and DSP algorithms.

Find more detailed information about the project and it's at [docs/usage.md](docs/usage.md). An example implementation of it's functionality is provided by the [Editor](docs/editor.md) sub-project.

Development is happening at a sporadic pace. Some ideas for future features are noted at [docs/roadmap.md](docs/roadmap.md)

The name Yo is based on the phonetic approximation of the korean word for fox (여우).

## Build Instructions <a name="build"></a>

More detailed instructions on how to build the project can be found at [docs/build.md](docs/build.md).
The short and sweet of it goes as follows:

> git clone --recurse-submodules https://github.com/fuzzblob/YoAudio.git

Use CMake to generate the project - then compile with a C++20 toolchain.

## Acknowledgements<a name="acknowledge"></a>

The initial development was inspired by this [Simple SDL2 Audio example](https://github.com/jakebesworth/Simple-SDL2-Audio) but the direction of the software has since diverged from this very simple approach to playing audio.

A huge thank you to [Jeremy Tammik](https://github.com/jeremytammik) for helping me setup some basics, specifically CMake.
