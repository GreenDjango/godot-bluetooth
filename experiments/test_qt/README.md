# Bluetooth test with QT5

## Linux

> Need `qt5-qtconnectivity-devel` package and `cmake` >= 3.1.0

```shell
cd ./experiments/test_qt
cmake .
make
```

If CAP_NET_ADMIN permission is missing:

> *qt.bluetooth.bluez: Missing CAP_NET_ADMIN permission. Cannot determine whether a found address is of random or public type.*

Run `sudo setcap "cap_net_raw,cap_net_admin+eip" ./gblue.out`