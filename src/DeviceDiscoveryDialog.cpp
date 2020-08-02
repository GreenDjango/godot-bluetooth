/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** DeviceDiscoveryDialog.cpp
*/

#include "moc_DeviceDiscoveryDialog.cpp"
#include "DeviceDiscoveryDialog.hpp"

/// --- ServiceDiscoveryDialog ---
ServiceDiscoveryDialog::ServiceDiscoveryDialog(const QBluetoothAddress& address)
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
}

/// --- DeviceDiscoveryDialog ---
DeviceDiscoveryDialog::DeviceDiscoveryDialog()
    : localDevice(new QBluetoothLocalDevice)
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();

    //connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &DeviceDiscoveryDialog::addDevice);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &DeviceDiscoveryDialog::scanFinished);

    /*connect(localDevice, SIGNAL(hostModeStateChanged(QBluetoothLocalDevice::HostMode)),
            this, SLOT(hostModeStateChanged(QBluetoothLocalDevice::HostMode)));*/

    hostModeStateChanged(localDevice->hostMode());
    //connect(localDevice, SIGNAL(pairingFinished(QBluetoothAddress, QBluetoothLocalDevice::Pairing)), this, SLOT(pairingDone(QBluetoothAddress, QBluetoothLocalDevice::Pairing)));
}

DeviceDiscoveryDialog::~DeviceDiscoveryDialog()
{
    delete discoveryAgent;
}

void DeviceDiscoveryDialog::addDevice(const QBluetoothDeviceInfo& info)
{
    QString label = QString("%1 %2").arg(info.address().toString()).arg(info.name());
    QBluetoothLocalDevice::Pairing pairingStatus = localDevice->pairingStatus(info.address());
    std::cout << label.toStdString();
    if (pairingStatus == QBluetoothLocalDevice::Paired || pairingStatus == QBluetoothLocalDevice::AuthorizedPaired)
        std::cout << " (Paired)\n";
    else
        std::cout << " (Unpaired)\n";
    //ServiceDiscoveryDialog d(info.address());
}

void DeviceDiscoveryDialog::on_power_clicked(bool clicked)
{
    if (clicked)
        localDevice->powerOn();
    else
        localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
}

void DeviceDiscoveryDialog::on_discoverable_clicked(bool clicked)
{
    if (clicked)
        localDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    else
        localDevice->setHostMode(QBluetoothLocalDevice::HostConnectable);
}

void DeviceDiscoveryDialog::startScan()
{
    discoveryAgent->start();
}

void DeviceDiscoveryDialog::scanFinished()
{
    std::cout << "finish\n";
    emit finished();
    //localDevice->requestPairing(address, QBluetoothLocalDevice::Paired);
}

void DeviceDiscoveryDialog::setGeneralUnlimited(bool unlimited)
{
    if (unlimited)
        discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry);
    else
        discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::LimitedInquiry);
}

void DeviceDiscoveryDialog::hostModeStateChanged(QBluetoothLocalDevice::HostMode)
{
    /*if (mode != QBluetoothLocalDevice::HostPoweredOff)
            1; //ui->power->setChecked(true);
        else
            1; //ui->power->setChecked(false);

        if (mode == QBluetoothLocalDevice::HostDiscoverable)
            1; //ui->discoverable->setChecked(true);
        else
            1; //ui->discoverable->setChecked(false);*/
}
