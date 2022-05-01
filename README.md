# <img src="icons/icon_bluetooth.svg" alt="GGJ icon" width="50" style="margin-bottom: -6px;"/> Bluetooth Module

### (WIP) [Bluetooth®](https://www.bluetooth.com) module for godot 3.x

|**Platform**|**Support**|**Dependencies**|
|---:|:---:|---:|
|🪟 windows|❌|
|🐧x11|✔️|[sdbus-c++](https://github.com/Kistler-Group/sdbus-cpp) (libsystemd >= 0.20)
|🍎osx|❌|
|📱android|❌|

### 💾 Setup:
#### via submodule
1. `git clone --recursive --single-branch --branch 3.4-stable https://github.com/godotengine/godot.git`
2. open a terminal, `cd godot`, and run the following commands:  
`cd modules`  
`git submodule add https://github.com/GreenDjango/godot-bluetooth.git bluetooth`  
`cd ../`  
`git submodule update --init --recursive`

> You can change `--branch 3.4-stable` to other release like `3.3.4-stable`

#### via download
1. Download a [godot source code release](https://github.com/godotengine/godot/releases)
2. Download the zip ([godot-bluetooth-master](https://github.com/GreenDjango/godot-bluetooth/archive/master.zip)) and extract it into the `godot/modules/` folder.

### 🛠 Building

#### From linux

<details>
      <summary>🐧<b>Ubuntu</b> dependencies</summary>

      sudo apt install sdbus-c++-dev    # 18
      sudo apt install libsdbus-c++-dev # 20
</details>
<details>
      <summary>🐧<b>Fedora</b> dependencies</summary>

      sudo dnf install sdbus-cpp-devel
</details>
<details>
      <summary>🐧<b>Arch</b> dependencies</summary>

      sudo yay -Syu sdbus-cpp
</details>

For compiling Godot under Linux or other Unix variants, [the following is required](https://docs.godotengine.org/en/3.4/development/compiling/compiling_for_x11.html#distro-specific-one-liners).

Then, you can use `scons -j8 platform=x11` for start compiling, where `-j8` is the number of threads.

Godot binary is create in the `bin/` folder like `bin/godot.x11.tools.64`.

> If you want to use separate editor settings for your own Godot builds and official releases, you can enable Self-contained mode by creating a file called `._sc_` or `_sc_` in the `bin/` folder.

#### From other

[See the official docs](https://docs.godotengine.org/en/3.4/development/compiling/) for compilation instructions for every supported platform.

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
