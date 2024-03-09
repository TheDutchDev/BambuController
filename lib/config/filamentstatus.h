#ifndef FILAMENTSTATUS_H
#define FILAMENTSTATUS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <defs.h>

enum class EFilamentType
{
    PLA,
    PETG,
    TPU,
    PA,
    PC,
    ABS,
    ASA
};

class FilamentStatus
{
public:
    char color[8];
    EFilamentType type;
    char typeString[32];

    void toJson(JsonDocument &json)
    {
        json["printer"]["filament"]["status"]["color"] = color;
        json["printer"]["filament"]["status"]["type"] = (int)type;
    }

    void resetToDefaults()
    {
        strcpy(color, "");
        type = EFilamentType::PLA;
    }

    void update(JsonDocument &json)
    {
        if (!json.containsKey("print") ||
            !json["print"].containsKey("ams") ||
            !json["print"]["ams"].containsKey("ams") ||
            !json["print"]["ams"].containsKey("tray_now"))
            return;

        uint8_t currentTrayId = json["print"]["ams"]["tray_now"].as<uint8_t>();

        uint8_t amsId = floor(currentTrayId / 4);
        uint8_t amsTrayId = floor(currentTrayId % 4);

        auto currentTrayData = json["print"]["ams"]["ams"][amsId]["tray"][amsTrayId];

        //(homeflag >> 23) & 0x01

        if (currentTrayData.containsKey("tray_color"))
            strcpy(color, currentTrayData["tray_color"]);

        if (currentTrayData.containsKey("tray_type"))
            convertType(currentTrayData["tray_type"]);

        Serial.print("Current filament -> type: ");
        Serial.print(typeString);
        Serial.print(", color: #");
        Serial.println(color);
    }

private:
    void convertType(const char *trayType)
    {
        strcpy(typeString, trayType);

        if (trayType == "PETG")
            type = EFilamentType::PETG;

        else if (trayType == "TPU")
            type = EFilamentType::TPU;

        else if (trayType == "PA")
            type = EFilamentType::PA;

        else if (trayType == "PC")
            type = EFilamentType::PC;

        else if (trayType == "ABS")
            type = EFilamentType::ABS;

        else if (trayType == "ASA")
            type = EFilamentType::ASA;

        else
            type = EFilamentType::PLA;
    }
};

#endif