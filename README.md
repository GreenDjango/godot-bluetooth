# <img src="icons/icon_bluetooth.svg" alt="GGJ icon" width="50" style="margin-bottom: -6px;"/> Bluetooth Module

### [Bluetooth®](https://www.bluetooth.com) module for godot

|**Platform**|**Support**|**Dependencies**|
|---:|:---:|---:|
|🪟 windows|❌|
|🐧x11|✔️|[sdbus-c++](https://github.com/Kistler-Group/sdbus-cpp) (libsystemd >= 0.20)
|🍎osx|❌|
|📱android|❌|

### 💾 Setup:
#### via submodule
1. `git clone --recursive https://github.com/godotengine/godot.git`
2. open a terminal, `cd godot`, and run the following commands:  
`cd modules`  
`git submodule add https://github.com/GreenDjango/godot-bluetooth.git bluetooth`  
`git submodule update --init --recursive` 

#### via download
1. Download a [godot source code release](https://github.com/godotengine/godot/releases)
2. Download the zip ([godot-bluetooth-master](https://github.com/GreenDjango/godot-bluetooth/archive/master.zip)) and extract it into the `godot/modules/` folder.

### 🛠 Building
[See the official docs](https://docs.godotengine.org/en/latest/development/compiling/) for compilation instructions for every supported platform.

<details>
      <summary>🐧<b>Ubuntu</b> dependencies</summary>

      sudo apt install sdbus-c++-dev
</details>
<details>
      <summary>🐧<b>Fedora</b> dependencies</summary>

      sudo dnf install sdbus-cpp-devel
</details>

### 🎮 Godot
|**Class**|**Description**|**Access**|
|---:|:---|:---|
|`Bluetooth`|Basic Bluetooth manger (switch BT on, scan nearby device, etc)|Node & GDScrip
|`NetworkedMultiplayerBt`|its purpose is to provide wrapper methods for all Bluetooth calls with a NetworkedMultiplayerPeer godot type friendly interface.|GDScrip

### TODO
- doc_classes need to be written for GDScript documentation.
- Provide some other singleton class.
- Use https://code.qt.io/cgit/qt/qtconnectivity.git/tree/src/bluetooth code for example
- [LINUX] build pipeline untested.
- [WIN] build pipeline untested.
- [OSX] build pipeline untested.
- [ANDROID] build pipeline untested.
