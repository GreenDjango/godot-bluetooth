/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** BluetoothModule.cpp
*/

#include "BluetoothModule.hpp"
#include "moc_BluetoothModule.cpp"

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
    if (rfcommServer)
        stopServer();
    if (socketClient)
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
    rfcommServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(rfcommServer, &QBluetoothServer::newConnection, this, &BluetoothModule::newConnectionServer);
    bool result = rfcommServer->listen(localDevice->address());
    if (!result) {
        std::cout << "[SERVER] Cannot bind chat server to" << localDevice->address().toString().toStdString();
        return;
    }
    std::cout << "[SERVER] " << rfcommServer->serverAddress().toString().toStdString() << " " << rfcommServer->serverPort() << "\n";

    //! [Service name, description and provider]
    serviceInfoServer.setAttribute(QBluetoothServiceInfo::ServiceName, tr("Bt Godot Server"));
    serviceInfoServer.setAttribute(QBluetoothServiceInfo::ServiceDescription, tr("Godot game engine server"));
    serviceInfoServer.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("godotengine.org"));
    //! [Service UUID set]
    static const QLatin1String serviceUuid("5ddaae8e-223d-45fe-98fb-ee2ec12f5a64");
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
    if (!rfcommServer)
        return;
    serviceInfoServer.unregisterService();
    //qDeleteAll(clientSockets);
    delete rfcommServer;
    rfcommServer = nullptr;
}

void BluetoothModule::startClient(const QBluetoothServiceInfo& remoteService)
{
    if (socketClient)
        return;
    // Connect to service
    socketClient = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
    std::cout << "Create socket";
    socketClient->connectToService(remoteService);
    std::cout << "ConnectToService done";

    connect(socketClient, &QBluetoothSocket::readyRead, this, &BluetoothModule::readSocket);
    /*
    connect(socketClient, &QBluetoothSocket::connected, this, &BluetoothModule::connected);
    connect(socketClient, &QBluetoothSocket::disconnected, this, &BluetoothModule::disconnected);
    connect(socketClient, SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(socketError(QBluetoothSocket::SocketError)));
    */
}

void BluetoothModule::stopClient()
{
    if (!socketClient)
        return;
    delete socketClient;
    socketClient = nullptr;
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

void BluetoothModule::newConnectionServer()
{
    QBluetoothSocket* socket = rfcommServer->nextPendingConnection();
    if (!socket)
        return;

    std::cout << "[SERVER] New Client, " << socket->peerAddress().toString().toStdString() << " " << socket->peerName().toStdString() << "\n";
    //connect(socket, &QBluetoothSocket::readyRead, this, &ChatServer::readSocket);
    //connect(socket, &QBluetoothSocket::disconnected, this, QOverload<>::of(&ChatServer::clientDisconnected));
    clientSockets.append(socket);
    QByteArray text = tr("Hello !!!").toUtf8() + '\n';
    socket->write(text);
    //emit clientConnected(socket->peerName());
}

void BluetoothModule::readSocket()
{
    if (!socketClient)
        return;

    while (socketClient->canReadLine()) {
        QByteArray line = socketClient->readLine();
        std::cout << socketClient->peerName().toStdString() << ": " << QString::fromUtf8(line.constData(), line.length()).toStdString() << "\n";
        //emit messageReceived();
    }
}