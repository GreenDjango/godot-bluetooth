# Bluetooth test with QT6

> `git clone --depth=1 --recursive --single-branch --branch v6.3.1 git@github.com:qt/qtconnectivity.git`

## Linux

> Need `qt6-qtconnectivity-devel` package and `cmake` >= 3.1.0


```shell
cd ./experiments/test_qt
cmake .
make
```

If CAP_NET_ADMIN permission is missing:

> *qt.bluetooth.bluez: Missing CAP_NET_ADMIN permission. Cannot determine whether a found address is of random or public type.*

Run `sudo setcap "cap_net_raw,cap_net_admin+eip" ./gblue.out`

## Ref

- [Does Qt-based dll can be used by another non qt-based app](https://forum.qt.io/topic/91101/does-qt-based-dll-can-be-used-by-another-non-qt-based-app-without-qapplication)
- [How to avoid Qt app.exec() blocking main thread](https://stackoverflow.com/questions/22289423/how-to-avoid-qt-app-exec-blocking-main-thread/22290909#22290909)
- [At last, let's build Qt statically](https://decovar.dev/blog/2018/02/17/build-qt-statically/)
- [Precompiled x64 Qt 5/6 library in minimalistic configuration for Windows.](https://github.com/martinrotter/qt-minimalistic-builds)


## Build QT6 from source

> Download source here: https://download.qt.io/official_releases/qt/6.3/6.3.1/single/qt-everywhere-src-6.3.1.zip

```shell
mkdir build && cd build

../configure -static -release -no-pch -prefix "~/qt/bin/6.3.1-static" -nomake examples -nomake tests -nomake benchmarks -nomake manual-tests -nomake minimal-static-tests -submodules qtconnectivity -skip qtimageformats -skip qtlanguageserver -skip qtshadertools -skip qtsvg -no-gui -no-widgets
# -ltcg -optimize-size

cmake --build . --parallel
cmake --install .

# PATH=/usr/local/Qt-%VERSION%/bin:$PATH
# export PATH
```