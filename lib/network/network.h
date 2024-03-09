#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>

class Network
{
public:
    Network();
    bool connect(const char *ssid, const char *password, const char *hostName);
    void setupAccessPoint(const char *ssid = "bambu", const char *password = "bambu123");
    void loop();
};

#endif