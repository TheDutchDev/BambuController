#include <bambu.h>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

Bambu::Bambu(PrinterConfig *printer, PrinterStatus *status) : printerConfig(printer), printerStatus(status)
{
}

void Bambu::setup()
{
    clientId = "BBLC-" + String(random(), HEX);

    setBufferSize(MAX_BUFFER_SIZE);
    setServer(printerConfig->ipAddress, 8883);

    setCallback(std::bind(&Bambu::onMqttMessage, this, _1, _2, _3));

    init(clientId.c_str(), "bblp", printerConfig->accessCode);
}

void Bambu::reconnect()
{
    printerStatus->online = false;
    Mqtt::reconnect();
}

void Bambu::onConnected()
{
    subscribeToTopics();
    printerStatus->online = true;
}

void Bambu::onMqttMessage(char *topic, uint8_t *payload, unsigned int length)
{
    DynamicJsonDocument jsonDoc(MAX_DOCUMENT_SIZE);

    auto error = deserializeJson(jsonDoc, payload, length, DeserializationOption::Filter(getPayloadFilter()));
    if (!error)
    {
        onMqttJsonDataReceived(jsonDoc);
    }
    else
    {
        Serial.println(F("Deserialize error while parsing mqtt"));
    }
}

void Bambu::onMqttJsonDataReceived(DynamicJsonDocument jsonDocument)
{
    // Serial.print("json doc received: ");
    // serializeJson(jsonDocument, Serial);
    // Serial.println();

    printerStatus->update(jsonDocument);
    emit("data", jsonDocument);
}

StaticJsonDocument<64> Bambu::getPayloadFilter()
{
    StaticJsonDocument<64> payloadFilter;
    payloadFilter["print"] = true;
    return payloadFilter;
}

void Bambu::subscribeToTopics()
{
    deviceTopic = String("device/") + printerConfig->serialNumber;
    reportTopic = deviceTopic + String("/report");
    requestTopic = deviceTopic + String("/request");

    subscribe(reportTopic.c_str());
}