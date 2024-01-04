#ifndef ESPWEBSERVER_H
#define ESPWEBSERVER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "defs.h"
#include <WiFi.h>
#include <LittleFS.h>
#include <AsyncTCP.h>
#include "AsyncJson.h"
#include <config.h>
#include <ESPAsyncWebServer.h>
#include <eventemitter.h>
#include <functional>

class EspWebServer : public EventEmitter<DynamicJsonDocument>
{
public:
    EspWebServer(uint16_t port, SystemConfig *sysConf);
    void begin();
    void loop();
    void broadcastWs(const char *payload);
    void onBambuPrinterData(DynamicJsonDocument jsonDoc);

private:
    void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    void onApiInfo(AsyncWebServerRequest *request);
    void onUpdate(AsyncWebServerRequest *request, JsonVariant &json);
    void onNotFound(AsyncWebServerRequest *request);
    void onReboot(AsyncWebServerRequest *request);
    void onOverrideLedColor(AsyncWebServerRequest *request, JsonVariant &json);
    void onFactoryReset(AsyncWebServerRequest *request);

    AsyncWebServer server;
    AsyncWebSocket ws;

    SystemConfig *systemConfig;
};

#endif