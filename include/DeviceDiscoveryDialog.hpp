/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** main.hpp
*/

#ifndef _MY_MAIN_HPP
#define _MY_MAIN_HPP

#include <QtBluetooth/qbluetoothaddress.h>
#include <QtBluetooth/qbluetoothdevicediscoveryagent.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothservicediscoveryagent.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothuuid.h>
#include <QtCore/QObject>
#include <iostream>

class ServiceDiscoveryDialog : public QObject {

public:
    ServiceDiscoveryDialog(const QBluetoothAddress& address);
    ~ServiceDiscoveryDialog();
    void addService(const QBluetoothServiceInfo& info);

private:
    QBluetoothServiceDiscoveryAgent* discoveryAgent;
};

class DeviceDiscoveryDialog : public QObject {
    Q_OBJECT
public:
    explicit DeviceDiscoveryDialog();
    ~DeviceDiscoveryDialog();

public slots:
    void addDevice(const QBluetoothDeviceInfo& info);
    void on_power_clicked(bool clicked);
    void on_discoverable_clicked(bool clicked);
    //void pairingDone(const QBluetoothAddress&, QBluetoothLocalDevice::Pairing);
    void startScan();

signals:
    void finished();

private slots:
    void scanFinished();
    void setGeneralUnlimited(bool unlimited);
    void hostModeStateChanged(QBluetoothLocalDevice::HostMode);

private:
    QBluetoothDeviceDiscoveryAgent* discoveryAgent;
    QBluetoothLocalDevice* localDevice;
};

#endif
