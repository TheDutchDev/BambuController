#ifndef BAMBU_H
#define BAMBU_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "defs.h"
#include <base/mqtt.h>
#include <functional>
#include <config.h>
#include <eventemitter.h>

class Bambu : public Mqtt, public EventEmitter<DynamicJsonDocument>
{
public:
    Bambu(PrinterConfig *printer, PrinterStatus *status);
    void setup();
    void reconnect() override;
    void onMqttMessage(char *topic, uint8_t *payload, unsigned int length);

protected:
    void onConnected() override;

private:
    void onMqttJsonDataReceived(DynamicJsonDocument jsonDocument);
    StaticJsonDocument<64> getPayloadFilter();

    void subscribeToTopics();

    String clientId;
    String deviceTopic;
    String reportTopic;
    String requestTopic;

    PrinterConfig *printerConfig;
    PrinterStatus *printerStatus;
};

#endif