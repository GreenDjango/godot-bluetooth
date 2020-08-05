/*
** GODOT PROJECT, 2020
** bluetooth module
** File description:
** main.hpp
*/

#ifndef _MY_RECEPTION_HPP
#define _MY_RECEPTION_HPP

#include <iostream>
#include <vector>

class ClassOfDevice {
public:
    ClassOfDevice(uint32_t cod) : majorDevClassIdx{-1}
    {
        if (cod >= 0x1000000) {
            std::cerr << "COD error: Too big\n";
        }
        //cod = parseInt(document.COD.result.value, 16);
        if (cod & 3) {
            std::cerr << "COD error: Format Type\n";
        }
        uint32_t SvcMask = 0xFFE000;
        uint32_t MajDevMask = 0x001F00;
        uint32_t MinDevMask = 0x0000FF;
        serviceClass = cod & SvcMask;
        majorDevClass = cod & MajDevMask;
        minorDevClass = cod & MinDevMask;

        // SERVICE CLASSE
        if (serviceClass & 0x00C000) {
            // is a error but why ?
            std::cerr << "COD error: ???\n";
        }
        if (cod & (1 << 13))
            serviceClassStr.push_back("Limited Discoverable Mode");
        // 14 and 15 are reserved
        if (cod & (1 << 16))
            serviceClassStr.push_back("Positioning");
        if (cod & (1 << 17))
            serviceClassStr.push_back("Networking");
        if (cod & (1 << 18))
            serviceClassStr.push_back("Rendering");
        if (cod & (1 << 19))
            serviceClassStr.push_back("Capturing");
        if (cod & (1 << 20))
            serviceClassStr.push_back("Object Transfer");
        if (cod & (1 << 21))
            serviceClassStr.push_back("Audio");
        if (cod & (1 << 22))
            serviceClassStr.push_back("Telephony");
        if (cod & (1 << 23))
            serviceClassStr.push_back("Information");

        // MAJOR DEVICE CLASSE
        const char* majorStr[11] = { "Miscellaneous", "Computer", "Phone", "LAN / Network Access Point",
            "Audio / Video", "Peripheral", "Imaging", "Wearable", "Toy", "Health", "Uncategorized" };
        int majDevIdx = majorDevClass == MajDevMask ? 10 : majorDevClass >> 8;
        if (majDevIdx > 10 || majDevIdx < 0) {
            std::cerr << "COD error: Bad Major Device Class\n";
        } else {
            majorDevClassIdx = majDevIdx;
            majorDevClassStr = majorStr[majDevIdx];
        }

        // MINOR DEVICE CLASSE
        const char* devComputer[7] = { "Uncategorized", "Desktop workstation", "Server-class computer", "Laptop", "Handheld PC / PDA (Clam Shell)", "Palm sized PC / PDA", "Wearable computer (watch sized)" };
        const char* DevPhone[6] = { "Uncategorized", "Cellular", "Cordless", "Smart phone", "Wired modem or voice gateway", "Common ISDN Access" };
        const char* DevLAN[8] = { "Fully Available", "1 - 17% utilized", "17 - 33% utilized", "33 - 50% utilized", "50 - 67% utilized", "67 - 83% utilized", "83 - 99% utilized", "No service available" };
        const char* DevAudioVideo[19] = { "Uncategorized", "Wearable headset device", "Hands-free device", "(Reserved)", "Microphone", "Loudspeaker", "Headphones", "Portable audio", "Car audio", "Set-top box", "HiFi Audio Device", "VCR", "Video Camera", "Camcorder", "Video Monitor", "Video Display and Loudspeaker", "Video Conferencing", "(Reserved)", "Gaming / Toy" };
        const char* DevPeripheral[4] = { "Not Keyboard / Not Pointing Device", "Keyboard", "Pointing device", "Combo Keyboard / Pointing device" };
        const char* DevPeripheralSub[7] = { "Uncategorized device", "Joystick", "Gamepad", "Remote control", "Sensing device", "Digitizer tablet", "Card Reader" };
        const char* DevImaging[4] = { "Display", "Camera", "Scanner", "Printer" };
        const char* DevWearable[6] = { "Undefined", "Wrist Watch", "Pager", "Jacket", "Helmet", "Glasses" };
        const char* DevToy[6] = { "Undefined", "Robot", "Vehicle", "Doll / Action Figure", "Controller", "Game" };
        const char* DevHealth[8] = { "Undefined", "Blood Pressure Monitor", "Thermometer", "Weighing Scale", "Glucose Monitor", "Pulse Oximeter", "Heart / Pulse Rate Monitor", "Health Data Display" };
        switch (majDevIdx) {
        case 1:
            if ((minorDevClass >> 2) > 6) {
                std::cerr << "COD error: Bad Minor Device Class\n";
            } else
                minorDevClassStr = devComputer[(minorDevClass >> 2)];
            break;
        case 2:
            if ((minorDevClass >> 2) > 5) {
                std::cerr << "COD error: Bad Minor Device Class\n";
            } else
                minorDevClassStr = DevPhone[(minorDevClass >> 2)];
            break;
        case 3:
            if ((minorDevClass >> 5) > 7) {
                std::cerr << "COD error: Bad Minor Device Class\n";
            } else
                minorDevClassStr = DevLAN[(minorDevClass >> 5)];
            break;
        case 4:
            if ((minorDevClass >> 2) > 18) {
                std::cerr << "COD error: Bad Minor Device Class\n";
            } else
                minorDevClassStr = DevAudioVideo[(minorDevClass >> 2)];
            break;
        case 5:
            // Two class
            if (((minorDevClass >> 6) & 3) > 3) {
                std::cerr << "COD error: Bad Minor Device Class\n";
            } else
                minorDevClassStr = DevPeripheral[(minorDevClass >> 6) & 3];
            if (((minorDevClass >> 2) & 15) > 6) {
                std::cerr << "COD error: Bad Minor Device Class\n";
            } else
                minorDevClassStr = minorDevClassStr + " | " + DevPeripheralSub[(minorDevClass >> 2) & 15];
            break;
        case 6:
            // Four class
            if (minorDevClass & (1 << 4)) {
                minorDevClassStr = DevImaging[0];
                minorDevClassStr += " ";
            }
            if (minorDevClass & (1 << 5)) {
                minorDevClassStr = DevImaging[1];
                minorDevClassStr += " ";
            }
            if (minorDevClass & (1 << 6)) {
                minorDevClassStr = DevImaging[2];
                minorDevClassStr += " ";
            }
            if (minorDevClass & (1 << 7)) {
                minorDevClassStr = DevImaging[3];
                minorDevClassStr += " ";
            }
            if (!minorDevClassStr.empty())
                minorDevClassStr.pop_back();
            break;
        case 7:
            if ((minorDevClass >> 2) > 5) {
                std::cerr << "COD error: Bad Minor Device Class\n";
            } else
                minorDevClassStr = DevWearable[(minorDevClass >> 2)];
            break;
        case 8:
            if ((minorDevClass >> 2) > 5) {
                std::cerr << "COD error: Bad Minor Device Class\n";
            } else
                minorDevClassStr = DevToy[(minorDevClass >> 2)];
            break;
        case 9:
            if ((minorDevClass >> 2) > 7) {
                std::cerr << "COD error: Bad Minor Device Class\n";
            } else
                minorDevClassStr = DevHealth[(minorDevClass >> 2)];
            break;
        default:
            if (minorDevClass != 0) {
                std::cerr << "COD error: Bad Minor Device Class\n";
            }
            break;
        }
    };
    std::vector<std::string> serviceClassStr;
    std::string majorDevClassStr;
    std::string minorDevClassStr;
    int majorDevClassIdx;

private:
    uint32_t serviceClass;
    uint32_t majorDevClass;
    uint32_t minorDevClass;
};

#endif
