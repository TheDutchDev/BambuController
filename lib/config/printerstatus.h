#ifndef PRINTERSTATUS_H
#define PRINTERSTATUS_H

#include <Arduino.h>
#include <ArduinoJson.h>

class PrinterStatus
{
public:
    String gcodeState = "IDLE";
    int stage = 0;
    bool ledState = true;
    bool online = false;
    bool finished = false;

    void toJson(DynamicJsonDocument &json)
    {
        json["printer"]["status"]["gcodeState"] = gcodeState;
        json["printer"]["status"]["stage"] = stage;
        json["printer"]["status"]["ledState"] = ledState;
        json["printer"]["status"]["online"] = online;
        json["printer"]["status"]["finished"] = finished;
    }

    void fromJson(DynamicJsonDocument &json)
    {
        gcodeState = json["printer"]["status"]["parsedHMS"].as<String>();
        stage = json["printer"]["status"]["stage"].as<int>();
        ledState = json["printer"]["status"]["ledState"].as<bool>();
        online = json["printer"]["status"]["online"].as<bool>();
        finished = json["printer"]["status"]["finished"].as<bool>();
    }

    void resetToDefaults()
    {
        gcodeState = "IDLE";
        stage = 0;
        ledState = true;
        online = false;
        finished = false;
    }

    void update(DynamicJsonDocument &json)
    {
        if (json["print"].containsKey("stg_cur"))
        {
            stage = json["print"]["stg_cur"];
        }

        if (json["print"].containsKey("gcode_state"))
        {
            gcodeState = json["print"]["gcode_state"].as<String>();
            finished = gcodeState == "FINISH";
        }

        if (json["print"].containsKey("lights_report"))
        {
            JsonArray lightsReport = json["print"]["lights_report"];

            for (auto light : lightsReport)
            {
                if (light["node"] == "chamber_light")
                {
                    ledState = light["mode"] == "on";
                    break;
                }
            }
        }
    }
};

#endif