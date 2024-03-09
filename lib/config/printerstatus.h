#ifndef PRINTERSTATUS_H
#define PRINTERSTATUS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FilamentStatus.h>

enum class EPrinterState
{
    Idle,
    Finished,
    Failed,
    Slicing,
    Running,
    Prepare
};

class PrinterStatus
{
public:
    String gcodeState = "IDLE";
    int stage = 0;
    bool ledState = true;
    bool online = false;
    EPrinterState state = EPrinterState::Idle;
    uint8_t percentageCompleted = 0;

    uint8_t currentTrayId = 0;
    FilamentStatus *filamentStatus;

    PrinterStatus()
    {
        filamentStatus = new FilamentStatus();
    }

    void toJson(JsonDocument &json)
    {
        json["printer"]["status"]["gcodeState"] = gcodeState;
        json["printer"]["status"]["stage"] = stage;
        json["printer"]["status"]["ledState"] = ledState;
        json["printer"]["status"]["online"] = online;
        json["printer"]["status"]["state"] = (int)state;
        json["printer"]["status"]["percentageCompleted"] = percentageCompleted;

        filamentStatus->toJson(json);
    }

    // void fromJson(JsonDocument &json)
    // {
    //     if (!json.containsKey("printer") || !json["printer"].containsKey("status"))
    //         return;

    //     if (json["printer"]["status"].containsKey("parsedHMS"))
    //         gcodeState = json["printer"]["status"]["parsedHMS"].as<String>();

    //     if (json["printer"]["status"].containsKey("stage"))
    //         stage = json["printer"]["status"]["stage"].as<int>();

    //     if (json["printer"]["status"].containsKey("ledState"))
    //         ledState = json["printer"]["status"]["ledState"].as<bool>();

    //     if (json["printer"]["status"].containsKey("online"))
    //         online = json["printer"]["status"]["online"].as<bool>();

    //     if (json["printer"]["status"].containsKey("state"))
    //         state = static_cast<EPrinterState>((int)json["printer"]["status"]["state"]);
    // }

    void resetToDefaults()
    {
        gcodeState = "IDLE";
        stage = 0;
        ledState = true;
        online = false;
        percentageCompleted = 0;
        updateState();

        filamentStatus->resetToDefaults();
    }

    void update(JsonDocument &json)
    {
        if (!json.containsKey("print"))
            return;

        if (json["print"].containsKey("stg_cur"))
        {
            stage = json["print"]["stg_cur"];
        }

        if (json["print"].containsKey("gcode_state"))
        {
            gcodeState = json["print"]["gcode_state"].as<String>();
            updateState();
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

        if (json["print"].containsKey("mc_percent"))
        {
            percentageCompleted = json["print"]["mc_percent"].as<uint8_t>();
        }

        filamentStatus->update(json);
    }

private:
    void updateState()
    {
        if (gcodeState == "FINISH")
            state = EPrinterState::Finished;

        else if (gcodeState == "FAILED")
            state = EPrinterState::Failed;

        else if (gcodeState == "SLICING")
            state = EPrinterState::Slicing;

        else if (gcodeState == "RUNNING")
            state = EPrinterState::Running;

        else if (gcodeState == "PREPARE")
            state = EPrinterState::Prepare;

        else
            state = EPrinterState::Idle;
    }
};

#endif