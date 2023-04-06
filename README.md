# YoAudioEditor

Depends on [YoAudio](https://github.com/chtammik/YoAudio) library.

This repo contains a simple sandbox to run the YoAudio engine and test it's capabilities. To use simply clone this repo into a folder adjacent to the YoAudio repo. On every build the editor will copy the YoAudio.dll to it's build folder.

## setup

- clone both repositories next to each other
- YoAudio
	- extract SDL2 dev libraries and headers to dependencies folder
	- run CMAKE or build script
	- compile the code to generate library files
- YoAudioEditor
	- run CMAKE (or m.bat for Visual Studio)
	- compile
	- run