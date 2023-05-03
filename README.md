# <img src="icons/icon_bluetooth.svg" alt="GGJ icon" width="50" style="margin-bottom: -6px;"/> Godot Bluetooth Module

### (WIP) [Bluetooth®](https://www.bluetooth.com) module for godot 4.x

|**Platform**|**Support**|**Dependencies**|
|---:|:---:|---:|
|🪟 windows|✔️|[Simple BLE](https://github.com/OpenBluetoothToolbox/SimpleBLE)
|🐧x11|✔️|[Simple BLE](https://github.com/OpenBluetoothToolbox/SimpleBLE)
|🍎osx|✔️|[Simple BLE](https://github.com/OpenBluetoothToolbox/SimpleBLE)
|📱android|❌|

### 💾 Setup:

```shell
git clone --depth 1 --recursive --single-branch --branch simple-ble https://github.com/GreenDjango/godot-bluetooth.git
# or if you already have clone the repository
cd godot-bluetooth/ && git submodule update --init --recursive
```

> You can change `.gitmodules` to other godot release like `4.1`

### 🛠 Building

#### From linux

<details>
      <summary>🐧<b>Ubuntu</b> dependencies</summary>

      sudo apt-get install libdbus-1-dev
</details>
<details>
      <summary>🐧<b>Fedora</b> dependencies</summary>

      sudo dnf install dbus-devel
</details>
<details>
      <summary>🐧<b>Arch</b> dependencies</summary>

      sudo yay -Syu dbus-devel
</details>

For compiling Godot under Linux or other Unix variants, [the following is required](https://docs.godotengine.org/en/4.0/development/compiling/compiling_for_x11.html#distro-specific-one-liners).

Then, you can use `scons -j8 platform=linux` for start compiling, where `-j8` is the number of threads.

> NOTE: See the `Makefile` for other build.

Dynamic library is create in the `addons/xxx/lib` folder like `addons/blueplug/lib/libblueplug.linux.template_debug.x86_64.so`.

You can now start Godot 4.x and open `demo` project for test the generated library.

> If you want to use separate editor settings for your own Godot builds and official releases, you can enable Self-contained mode by creating a file called `._sc_` or `_sc_` next to your Godot binary.

#### From other

[See the official docs](https://docs.godotengine.org/en/4.0/development/compiling/) for compilation instructions for every supported platform.

### 🎮 Godot
|**Class**|**Description**|**Access**|
|---:|:---|:---|
|`BluePlug`|Basic Bluetooth manger (switch BT on, scan nearby device, etc)|GDScript
|`BluePlugMultiplayerPeer`|Its purpose is to provide wrapper methods for all Bluetooth calls with a NetworkedMultiplayerPeer godot type friendly interface.|GDScript
|`BluePlugPacketPeer`|Godot internal use.|GDScript

### 🤝 Setup to contribute

VScode is a good IDE for this project.

For style, you need [ms-vscode.cpptools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools). Next create `.vscode/settings.json` and add this line `{ "C_Cpp.clang_format_style": "file" }`. Or use `clang-format-14` cli without vscode.

### TODO
- doc_classes need to be written for GDScript documentation.
- Provide some other singleton class.
- Use https://code.qt.io/cgit/qt/qtconnectivity.git/tree/src/bluetooth code for example
- [LINUX] build pipeline untested.
- [WIN] build pipeline untested.
- [OSX] build pipeline untested.
- [ANDROID] build pipeline untested.
