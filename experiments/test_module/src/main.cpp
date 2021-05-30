/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** main.cpp
*/

#include "main.h"
#include "module/bluez.h"

#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>

void onConcatenated(sdbus::Signal& signal)
{
    std::string concatenatedString;
    signal >> concatenatedString;

    std::cout << "Received signal with concatenated string " << concatenatedString << std::endl;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    BluetoothWrapper BTwrapper;

    //BTwrapper.set_powered(false);

    if (BTwrapper.is_powered())
        std::cout << "ON\n";
    else
        std::cout << "OFF\n";
    

    return 0;
    // Create proxy object for the concatenator object on the server side. Since here
    // we are creating the proxy instance without passing connection to it, the proxy
    // will create its own connection automatically, and it will be system bus connection.
    // busctl get-property org.bluez /org/bluez/hci0 org.bluez.Adapter1 Powered 
    const char* destinationName = "org.bluez";
    const char* objectPath = "/org/bluez/hci0";
    auto concatenatorProxy = sdbus::createProxy(destinationName, objectPath);

    // Let's subscribe for the signals
    const char* interfaceName = "org.bluez.Adapter1";
    std::cout << concatenatorProxy->getProperty("Address").onInterface(interfaceName).get<std::string>() << std::endl;

    return 0;

    concatenatorProxy->registerSignalHandler(interfaceName, "Address", &onConcatenated);
    concatenatorProxy->finishRegistration();

    // std::vector<int> numbers = {1, 2, 3};
    // std::string separator = ":";

    // Invoke concatenate on given interface of the object
    {
        auto method = concatenatorProxy->createMethodCall(interfaceName, "Address");
        // method << numbers << separator;
        auto reply = concatenatorProxy->callMethod(method);
        std::string result;
        reply >> result;
        std::cout << result << std::endl;
    }

    // Invoke concatenate again, this time with no numbers and we shall get an error
    /*{
        auto method = concatenatorProxy->createMethodCall(interfaceName, "concatenate");
        method << std::vector<int>() << separator;
        try
        {
            auto reply = concatenatorProxy->callMethod(method);
            assert(false);
        }
        catch(const sdbus::Error& e)
        {
            std::cerr << "Got concatenate error " << e.getName() << " with message " << e.getMessage() << std::endl;
        }
    }*/

    // Give sufficient time to receive 'concatenated' signal from the first concatenate invocation
    sleep(1);

    return 0;
}