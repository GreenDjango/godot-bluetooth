/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** main.cpp
*/

#include "DeviceDiscoveryDialog.hpp"
#include <QtCore/qloggingcategory.h>
#include <QtWidgets/QApplication>

int main(int argc, char** argv)
{
    //QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));
    QApplication app(argc, argv);
    DeviceDiscoveryDialog d;
    d.startScan();
    app.connect(&d, &DeviceDiscoveryDialog::finished, &app, &QApplication::quit);
    return app.exec();
}