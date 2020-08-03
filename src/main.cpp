/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** main.cpp
*/

#include "BluetoothModule.hpp"
#include <QtCore/qloggingcategory.h>
#include <QtWidgets/QApplication>

int main(int argc, char** argv)
{
    //QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));
    QApplication app(argc, argv);
    BluetoothModule m;
    app.connect(&m, &BluetoothModule::finished, &app, &QApplication::quit);
    m.startServer();
    //74:29:AF:80:20:80
    return app.exec();
}