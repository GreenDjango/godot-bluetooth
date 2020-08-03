/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** BluetoothModule.hpp
*/

#ifndef _MY_SERVICEDISCOVERYDIALOG_HPP
#define _MY_SERVICEDISCOVERYDIALOG_HPP

#include <QtBluetooth/qbluetoothaddress.h>
#include <QtBluetooth/qbluetoothdevicediscoveryagent.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothservicediscoveryagent.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothuuid.h>
#include <QtBluetooth/QBluetoothServer>
#include <QtCore/QObject>
#include <iostream>
#include <map>

#define BT_SERVICE_NAME "Bt Godot Server"
#define BT_SERVICE_UUID "5ddaae8e-223d-45fe-98fb-ee2ec12f5a64"

/*class ServiceDiscoveryDialog : public QObject {

public:
    ServiceDiscoveryDialog(const QBluetoothAddress& address);
    ~ServiceDiscoveryDialog();
    void addService(const QBluetoothServiceInfo& info);

private:
    QBluetoothServiceDiscoveryAgent* discoveryAgent;
};*/

class BluetoothModule : public QObject {
    Q_OBJECT
public:
    explicit BluetoothModule();
    ~BluetoothModule();
    void switchPower(bool on = true);
    void switchDiscoverable(bool on = true);
    void setGeneralUnlimited(bool unlimited = true);
    void startServer();
    void stopServer();
    void startClient();
    void stopClient();
    void startScan();

signals:
    void finished();

private slots:
    void scanFinished();
    void scanError(QBluetoothDeviceDiscoveryAgent::Error);
    void onNewDevice(const QBluetoothDeviceInfo& info);
    //void pairingDone(const QBluetoothAddress&, QBluetoothLocalDevice::Pairing);
    void hostModeStateChanged(QBluetoothLocalDevice::HostMode);
    void newConnectionServer();
    void readSocket();

private:
    QBluetoothDeviceDiscoveryAgent* discoveryAgent;
    QBluetoothLocalDevice* localDevice;
    std::map<QBluetoothAddress, QBluetoothDeviceInfo> remoteDevices;
    QBluetoothServer *rfcommServer = nullptr;
    QBluetoothServiceInfo serviceInfoServer;
    QList<QBluetoothSocket *> clientSockets;
    QBluetoothSocket *socketClient = nullptr;
};

#endif
