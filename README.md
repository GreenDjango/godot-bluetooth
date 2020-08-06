# Bluetooth Module

### [Bluetooth®](https://www.bluetooth.com) module for godot

## Setup:
### via submodule
1. `git clone --recursive https://github.com/godotengine/godot.git` or download a [godot source code release](https://github.com/godotengine/godot/releases)
2. open a terminal, `cd godot`, and run the following commands:  
`cd modules`  
`git submodule add https://github.com/GreenDjango/godot-bluetooth.git`  
`git submodule update --init --recursive`

### via download 
- Download the zip ([godot-bluetooth-master](https://github.com/GreenDjango/godot-bluetooth/archive/master.zip)) and extract it into the godot/modules-folder.

## Building
[See the official docs](https://docs.godotengine.org/en/latest/development/compiling/) for compilation instructions for every supported platform.

## Godot
- `NetworkedMultiplayerBt` ~~is a singleton class~~, its purpose is to provide wrapper methods for all Bluetooth calls with a NetworkedMultiplayerPeer godot type friendly interface. For access from GDScrip

## TODO
- doc_classes need to be written for GDScript documentation.
- Provide some other singleton class.
- Use https://code.qt.io/cgit/qt/qtconnectivity.git/tree/src/bluetooth code rather than Qt5
- [WIN] build pipeline untested.
- [OSX] build pipeline untested.
- [ANDROID] build pipeline untested.