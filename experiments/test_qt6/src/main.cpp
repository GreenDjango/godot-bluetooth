/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** main.cpp
*/

#include <QProcess>
#include <QtBluetooth/QtBluetooth>
#include <iostream>
// #include <QtWidgets/QApplication>

int main(int argc, char **argv) {
	QBluetoothLocalDevice localDevice;
	QString localDeviceName;

	// Check if Bluetooth is available on this device
	if (localDevice.isValid()) {
		// Turn Bluetooth on
		localDevice.powerOn();
#if defined(Q_OS_LINUX)
		auto res = QProcess::execute("bluetooth", { "on" });
#endif
		// localDevice.setHostMode(QBluetoothLocalDevice::HostPoweredOff);

		// Read local device name
		// localDeviceName = localDevice.name();
		// std::cout << localDeviceName.toStdString() << "\n";

		// Make it visible to others
		// localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

		// Get connected devices
		// QList<QBluetoothAddress> remotes;
		// remotes = localDevice.connectedDevices();
	}
}
