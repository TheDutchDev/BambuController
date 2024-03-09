#include <Arduino.h>
#include <init.h>
#include <globals.h>
#include <functional>

using std::placeholders::_1;

void Core0(void *parameter)
{
    for (;;)
    {
        server->loop();
        delay(50);
        // vTaskDelay(10);
    }
}
void Core1(void *parameter)
{
    for (;;)
    {
        delay(50);
        // Serial.println( "core1" );
    }
}

void setup()
{
    Serial.begin(115200);
    ledStrip = new LedStrip(printerStatus, LED_STRIP_RED_PIN, LED_STRIP_GREEN_PIN, LED_STRIP_BLUE_PIN, LED_STRIP_WHITE_PIN, LED_STRIP_YELLOW_PIN);
    ledStrip->setColor(0, 0, 0, 0, 0, 0);

    ota->setup();

    if (!fileSystem->mount())
    {
        Serial.println("[FATAL] Failed to mount filesystem!");
        ESP.restart();
        return;
    }

    if (!fileSystem->load())
    {
        network->setupAccessPoint(AP_SSID, AP_PASSWORD);
        factoryDefaults->reload();
    }
    else
    {
        systemConfig->state = EState::Active;
        if (!network->connect(networkConfig->ssid, networkConfig->password, networkConfig->hostName))
        {
            Serial.println("wiping and restarting..");
            fileSystem->erase();
            WiFi.disconnect();
            ESP.restart();
        }

        bambu->setup();
        bambu->on("data", std::bind(&EspWebServer::onBambuPrinterData, server, _1));
        bambu->on("data", std::bind(&LedStrip::onBambuPrinterData, ledStrip, _1));

        fileSystem->save();

        bentoBox = new BentoBox(printerStatus, BENTO_BOX_PIN, BENTO_BOX_OFF_DELAY_MS);
    }

    server->on("update", std::bind(&FileSystem::update, fileSystem, _1));
    server->on("factory-reset", std::bind(&FileSystem::onFactoryReset, fileSystem, _1));

    server->begin();
    Init(Core0, Core1);
}

void runSetupLoop()
{
    Serial.print("state: ");
    Serial.print(systemConfig->state == EState::Setup ? "Setup" : "Active");

    Serial.print(", IP Address: ");
    Serial.print(WiFi.softAPIP());
    Serial.print(", hostname: ");
    Serial.println(WiFi.softAPgetHostname());

    network->loop();
    ledStrip->blink(1000);
}

void runActiveLoop()
{
    if (printerConfig->isConfigured())
    {
        if (!bambu->connected())
        {
            Serial.println("reconnecting bambu");
            bambu->reconnect();
        }

        // delay(1000);
        bambu->loop();
        bentoBox->stopIfTimeExpired();
    }

    if (printerStatus->online && printerStatus->state == EPrinterState::Idle)
        ledStrip->blink(5000);
}

void loop()
{
    switch (systemConfig->state)
    {
    case EState::Setup:
        runSetupLoop();
        break;

    case EState::Active:
        runActiveLoop();
        break;
    }

    ota->loop();
}