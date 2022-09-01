/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** BluetoothModule.cpp
*/

#include "BluetoothModule.hpp"
#include "moc_BluetoothModule.cpp"
#include <QtCore/qelapsedtimer.h>

QElapsedTimer t;

/// --- ServiceDiscoveryDialog ---
/*ServiceDiscoveryDialog::ServiceDiscoveryDialog(const QBluetoothAddress& address)
{
    QBluetoothLocalDevice localDevice;
    QBluetoothAddress adapterAddress = localDevice.address();
    discoveryAgent = new QBluetoothServiceDiscoveryAgent(adapterAddress);
    discoveryAgent->setRemoteAddress(address);
    //connect(discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)), this, SLOT(addService(QBluetoothServiceInfo)));
    //connect(discoveryAgent, SIGNAL(finished()), ui->status, SLOT(hide()));
    discoveryAgent->start();
}

ServiceDiscoveryDialog::~ServiceDiscoveryDialog()
{
    delete discoveryAgent;
}

void ServiceDiscoveryDialog::addService(const QBluetoothServiceInfo& info)
{
    if (info.serviceName().isEmpty())
        return;
    QString line = info.serviceName();
    if (!info.serviceDescription().isEmpty())
        line.append("\n\t" + info.serviceDescription());
    if (!info.serviceProvider().isEmpty())
        line.append("\n\t" + info.serviceProvider());
    std::cout << line.toStdString();
}*/

/// --- BluetoothModule ---
BluetoothModule::BluetoothModule()
{
    localDevice = new QBluetoothLocalDevice(this);
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BluetoothModule::onNewDevice);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BluetoothModule::scanFinished);
    connect(discoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)), this, SLOT(scanError(QBluetoothDeviceDiscoveryAgent::Error)));
    connect(localDevice, &QBluetoothLocalDevice::hostModeStateChanged, this, &BluetoothModule::hostModeStateChanged);
    hostModeStateChanged(localDevice->hostMode());
    //connect(localDevice, SIGNAL(pairingFinished(QBluetoothAddress, QBluetoothLocalDevice::Pairing)), this, SLOT(pairingDone(QBluetoothAddress, QBluetoothLocalDevice::Pairing)));
}

BluetoothModule::~BluetoothModule()
{
    if (hasServer())
        stopServer();
    if (hasClient())
        stopClient();
    delete discoveryAgent;
    delete localDevice;
}

void BluetoothModule::switchPower(bool on)
{
    if (on)
        localDevice->powerOn();
    else
        localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
}

void BluetoothModule::switchDiscoverable(bool on)
{
    if (on)
        localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    else
        localDevice->setHostMode(QBluetoothLocalDevice::HostConnectable);
}

void BluetoothModule::setGeneralUnlimited(bool unlimited)
{
    if (unlimited)
        discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry);
    else
        discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::LimitedInquiry);
}

void BluetoothModule::startServer()
{
    if (hasServer())
        return;
    switchPower();
    switchDiscoverable();
    rfcommServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(rfcommServer, &QBluetoothServer::newConnection, this, &BluetoothModule::newClientOnServer);
    bool result = rfcommServer->listen(localDevice->address());
    if (!result) {
        std::cout << "[SERVER] Cannot bind chat server to" << localDevice->address().toString().toStdString();
        return;
    }
    std::cout << "[SERVER] " << rfcommServer->serverAddress().toString().toStdString() << " " << rfcommServer->serverPort() << "\n";

    //! [Service name, description and provider]
    serviceInfoServer.setAttribute(QBluetoothServiceInfo::ServiceName, tr(BT_SERVICE_NAME));
    serviceInfoServer.setAttribute(QBluetoothServiceInfo::ServiceDescription, tr("Godot game engine server"));
    serviceInfoServer.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("godotengine.org"));
    //! [Service UUID set]
    static const QLatin1String serviceUuid(BT_SERVICE_UUID);
    serviceInfoServer.setServiceUuid(QBluetoothUuid(serviceUuid));
    //! [Service Discoverability]
    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    serviceInfoServer.setAttribute(QBluetoothServiceInfo::BrowseGroupList, publicBrowse);
    //! [Protocol descriptor list]
    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm)) << QVariant::fromValue(quint8(rfcommServer->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    serviceInfoServer.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList, protocolDescriptorList);
    //! [Register service]
    serviceInfoServer.registerService(localDevice->address());
}

void BluetoothModule::stopServer()
{
    if (!hasServer())
        return;
    serviceInfoServer.unregisterService();
    qDeleteAll(clientSockets);
    delete rfcommServer;
    rfcommServer = nullptr;
}

bool BluetoothModule::hasServer()
{
    return (rfcommServer != nullptr);
}

void BluetoothModule::startClient()
{
    if (hasClient())
        return;
    switchPower();
    // Connect to service
    QBluetoothServiceInfo remoteService;
    remoteService.setAttribute(QBluetoothServiceInfo::ServiceName, tr(BT_SERVICE_NAME));
    static const QLatin1String serviceUuid(BT_SERVICE_UUID);
    remoteService.setServiceUuid(QBluetoothUuid(serviceUuid));
    socketClient = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
    socketClient->connectToService(remoteService);

    connect(socketClient, &QBluetoothSocket::readyRead, this, &BluetoothModule::readSocketClient);
    connect(socketClient, &QBluetoothSocket::connected, this, &BluetoothModule::newServerOnClient);
    connect(socketClient, &QBluetoothSocket::disconnected, this, &BluetoothModule::lostServerOnClient);
    /*
    connect(socketClient, SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(socketError(QBluetoothSocket::SocketError)));
    */
}

void BluetoothModule::startClient(const std::string& host)
{
    if (hasClient())
        return;
    socketClient = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
    socketClient->connectToService(QBluetoothAddress(QString(host.c_str())), QBluetoothUuid(QLatin1String(BT_SERVICE_UUID)));

    connect(socketClient, &QBluetoothSocket::readyRead, this, &BluetoothModule::readSocketClient);
    connect(socketClient, &QBluetoothSocket::connected, this, &BluetoothModule::newServerOnClient);
    connect(socketClient, &QBluetoothSocket::disconnected, this, &BluetoothModule::lostServerOnClient);
    /*
    connect(socketClient, SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(socketError(QBluetoothSocket::SocketError)));
    */
}

void BluetoothModule::stopClient()
{
    if (!hasClient())
        return;
    delete socketClient;
    socketClient = nullptr;
}

bool BluetoothModule::hasClient()
{
    return (socketClient != nullptr);
}

void BluetoothModule::startScan()
{
    if (discoveryAgent->isActive())
        discoveryAgent->stop();
    discoveryAgent->start();
}

void BluetoothModule::scanFinished()
{
    std::cout << "finish\n";
    emit finished();
    //localDevice->requestPairing(address, QBluetoothLocalDevice::Paired);
}

void BluetoothModule::scanError(QBluetoothDeviceDiscoveryAgent::Error)
{
    std::cout << "Error: " << discoveryAgent->errorString().toStdString() << "\n";
    discoveryAgent->stop();
    scanFinished();
}

void BluetoothModule::onNewDevice(const QBluetoothDeviceInfo& info)
{
    QString label = QString("%1 %2").arg(info.address().toString()).arg(info.name());
    QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(info.address());
    std::cout << label.toStdString();
    if (pairingStatus == QBluetoothLocalDevice::Paired || pairingStatus == QBluetoothLocalDevice::AuthorizedPaired)
        std::cout << " (Paired)\n";
    else
        std::cout << " (Unpaired)\n";
    remoteDevices.insert(std::make_pair(info.address(), info));

    //ServiceDiscoveryDialog d(info.address());
}

void BluetoothModule::hostModeStateChanged(QBluetoothLocalDevice::HostMode mode)
{
    if (mode != QBluetoothLocalDevice::HostPoweredOff)
        std::cout << "Host Powered On\n";
    else
        std::cout << "Host Powered Off\n";

    if (mode == QBluetoothLocalDevice::HostDiscoverable)
        std::cout << "Host Discoverable\n";
    else
        std::cout << "Host Not Discoverable\n";
}

void BluetoothModule::newClientOnServer()
{
    if (!hasServer())
        return;
    QBluetoothSocket* socket = rfcommServer->nextPendingConnection();
    if (!socket)
        return;

    std::cout << "[SERVER] New Client, " << socket->peerAddress().toString().toStdString() << " " << socket->peerName().toStdString() << "\n";
    connect(socket, &QBluetoothSocket::readyRead, this, &BluetoothModule::readSocketServer);
    connect(socket, &QBluetoothSocket::disconnected, this, &BluetoothModule::lostClientOnServer);
    clientSockets.append(socket);

    QByteArray text = tr("Hello !!!").toUtf8() + '\n';
    socket->write(text);
    t.start();
    //emit clientConnected(socket->peerName());
}

void BluetoothModule::lostClientOnServer()
{
    QBluetoothSocket* socket = qobject_cast<QBluetoothSocket*>(sender());
    if (!socket)
        return;

    //emit clientDisconnected();
    clientSockets.removeOne(socket);
    socket->deleteLater();
}

void BluetoothModule::readSocketServer()
{
    if (!hasServer())
        return;
    QBluetoothSocket* socket = qobject_cast<QBluetoothSocket*>(sender());
    if (!socket)
        return;

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        std::cout << "[SERVER] (" << std::to_string(t.restart()) << "ms) "<<socket->peerName().toStdString() << ": " << QString::fromUtf8(line.constData(), line.length()).toStdString() << "\n";
        //emit messageReceived();
    }
    QByteArray text = tr("Hello !!!").toUtf8() + '\n';
    socket->write(text);
}

void BluetoothModule::newServerOnClient()
{
    //emit connected(socketClient->peerName());
}

void BluetoothModule::lostServerOnClient()
{
    std::cout << "Server disconnected\n";
}

void BluetoothModule::readSocketClient()
{
    if (!hasClient())
        return;

    while (socketClient->canReadLine()) {
        QByteArray line = socketClient->readLine();
        std::cout << socketClient->peerName().toStdString() << ": " << QString::fromUtf8(line.constData(), line.length()).toStdString() << "\n";
        //emit messageReceived();
    }
}