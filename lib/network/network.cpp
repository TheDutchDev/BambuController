#include <network.h>

Network::Network()
{
}

void Network::setupAccessPoint(const char *ssid, const char *password)
{
    IPAddress apIp(192, 168, 200, 1);

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIp, apIp, IPAddress(255, 255, 255, 0), IPAddress(192, 168, 200, 100));
    WiFi.softAP(ssid, password);
}

bool Network::connect(const char *ssid, const char *password, const char *hostName)
{
    int delayMs = 500;
    int attempts = 60;

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        attempts--;
        delay(delayMs);

        if (attempts <= 0)
        {
            return false;
        }
    }
    Serial.print("Connected on IP: ");
    Serial.println(WiFi.localIP());

#ifdef ARDUINO_ARCH_ESP32
    WiFi.setTxPower(WIFI_POWER_19_5dBm); // https://github.com/G6EJD/ESP32-8266-Adjust-WiFi-RF-Power-Output/blob/main/README.md
#endif

    // Set up mDNS
    if (!MDNS.begin(hostName))
    {
        Serial.println("Error setting up MDNS responder!");
        return false;
    }

    return true;
}

void Network::loop()
{
    delay(1000);
}