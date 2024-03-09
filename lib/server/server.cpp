#include <server.h>
#include <namespaces.h>

char last_modified[50];

EspWebServer::EspWebServer(uint16_t port, SystemConfig *sysConf)
    : server(port),
      ws("/ws"),
      systemConfig(sysConf)
{
}

void EspWebServer::begin()
{
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type, Content-Length");

    ws.onEvent(std::bind(&EspWebServer::onWebSocketEvent, this, _1, _2, _3, _4, _5, _6));
    server.addHandler(&ws);



    // Use to serve static HTML over filesystem. Not recommended for OTA
//    server.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");

    server.on("/", HTTP_GET, std::bind(&EspWebServer::onMainPage, this, _1));
    server.on("/api/info", HTTP_GET, std::bind(&EspWebServer::onApiInfo, this, _1));
    server.on("/api/reboot", HTTP_GET, std::bind(&EspWebServer::onReboot, this, _1));
    server.on("/api/factory-reset", HTTP_GET, std::bind(&EspWebServer::onFactoryReset, this, _1));

    AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/api/update", std::bind(&EspWebServer::onUpdate, this, _1, _2));
    handler->setMethod(HTTP_POST);
    server.addHandler(handler);

    // AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/api/leds/override", std::bind(&EspWebServer::onOverrideLedColor, this, _1, _2));
    // handler->setMethod(HTTP_POST);
    // server.addHandler(handler);

    server.onNotFound(std::bind(&EspWebServer::onNotFound, this, _1));

    // Start the server
    server.begin();
}

void EspWebServer::onNotFound(AsyncWebServerRequest *request)
{
    if (request->method() == HTTP_OPTIONS)
    {
        request->send(200);
    }
    else
    {
        request->send(404, "application/json", "{\"message\":\"Not found\"}");
    }
}

void EspWebServer::onMainPage(AsyncWebServerRequest *request)
{
    if (request->header("If-Modified-Since").indexOf(last_modified) > 0)
        return request->send(304);

    if (request->header("If-None-Match").equals(index_html_gz_sha))
        return request->send(304);

    AsyncWebServerResponse *response = request->beginResponse_P(
        200,
        "gzip",
        index_html_gz,
        index_html_gz_len
    );

    // Tell the browser the ontent is Gzipped
    response->addHeader("Content-Encoding", "gzip");

    // And set the last-modified datetime so we can check if we need to send it again next time or not
    response->addHeader("Last-Modified", last_modified);
    response->addHeader("ETag", index_html_gz_sha);

    request->send( response );
}

void EspWebServer::onApiInfo(AsyncWebServerRequest *request)
{
    String jsonString = getAllConfigAndStatusAsString();
    request->send(200, "text/json", jsonString);
}

void EspWebServer::onFactoryReset(AsyncWebServerRequest *request)
{
    JsonDocument jsonDoc;
    request->send(200);
    emit("factory-reset", jsonDoc );
}

void EspWebServer::onReboot(AsyncWebServerRequest *request)
{
    request->send(200);
    ws.closeAll();
    ESP.restart();
}

void EspWebServer::onUpdate(AsyncWebServerRequest *request, JsonVariant &json)
{
    // Print the received JSON data
    Serial.println("on update request");
    serializeJson(json, Serial);
    Serial.println();

    JsonDocument jsonDoc;

    if (json.is<JsonArray>())
    {
        jsonDoc = json.as<JsonArray>();
    }
    else if (json.is<JsonObject>())
    {
        jsonDoc = json.as<JsonObject>();
    }

    emit("update", jsonDoc);
    request->send(200, "text/plain", "done!");
}

void EspWebServer::onOverrideLedColor(AsyncWebServerRequest *request, JsonVariant &json)
{
    // Print the received JSON data
    Serial.println("on override led color");
    serializeJson(json, Serial);
    Serial.println();

    JsonDocument jsonDoc;

    if (json.is<JsonArray>())
    {
        jsonDoc = json.as<JsonArray>();
    }
    else if (json.is<JsonObject>())
    {
        jsonDoc = json.as<JsonObject>();
    }

    emit("overrideLeds", jsonDoc);
    request->send(200, "text/plain", "done!");
}

void EspWebServer::onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
        // client->printf("Hello Client %u :)", client->id());
        client->ping();
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        Serial.printf("ws[%s][%u] disconnect\n", server->url(), client->id());
    }
    else if (type == WS_EVT_ERROR)
    {
        Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t *)arg), (char *)data);
    }

    // region Unused
    //  else if (type == WS_EVT_PONG)
    //  {
    //      Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char *)data : "");
    //  }
    //  else if (type == WS_EVT_DATA)
    //  {
    //      AwsFrameInfo *info = (AwsFrameInfo *)arg;
    //      String msg = "";
    //      if (info->final && info->index == 0 && info->len == len)
    //      {
    //          // the whole message is in a single frame and we got all of it's data
    //          Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT) ? "text" : "binary", info->len);
    //         if (info->opcode == WS_TEXT)
    //         {
    //             for (size_t i = 0; i < info->len; i++)
    //             {
    //                 msg += (char)data[i];
    //             }
    //         }
    //         else
    //         {
    //             char buff[3];
    //             for (size_t i = 0; i < info->len; i++)
    //             {
    //                 sprintf(buff, "%02x ", (uint8_t)data[i]);
    //                 msg += buff;
    //             }
    //         }
    //         Serial.printf("%s\n", msg.c_str());
    //         if (info->opcode == WS_TEXT)
    //             client->text("I got your text message");
    //         else
    //             client->binary("I got your binary message");
    //     }
    //     else
    //     {
    //         // message is comprised of multiple frames or the frame is split into multiple packets
    //         if (info->index == 0)
    //         {
    //             if (info->num == 0)
    //                 Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
    //             Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
    //         }
    //         Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + len);
    //         if (info->opcode == WS_TEXT)
    //         {
    //             for (size_t i = 0; i < len; i++)
    //             {
    //                 msg += (char)data[i];
    //             }
    //         }
    //         else
    //         {
    //             char buff[3];
    //             for (size_t i = 0; i < len; i++)
    //             {
    //                 sprintf(buff, "%02x ", (uint8_t)data[i]);
    //                 msg += buff;
    //             }
    //         }
    //         Serial.printf("%s\n", msg.c_str());
    //         if ((info->index + len) == info->len)
    //         {
    //             Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
    //             if (info->final)
    //             {
    //                 Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
    //                 if (info->message_opcode == WS_TEXT)
    //                     client->text("I got your text message");
    //                 else
    //                     client->binary("I got your binary message");
    //             }
    //         }
    //     }
    // }
    // endregion
}

void EspWebServer::loop()
{
    ws.cleanupClients();
}

void EspWebServer::broadcastWs(const char *payload)
{
    ws.textAll(payload);
}

void EspWebServer::onBambuPrinterData(JsonDocument jsonDoc)
{
    String jsonString = getAllConfigAndStatusAsString();
    // String jsonString;
    // serializeJson(jsonDoc, jsonString);
    ws.textAll(jsonString);
}

String EspWebServer::getAllConfigAndStatusAsString()
{
    JsonDocument json;
    systemConfig->toJson(json);
    printerConfig->toJson(json);
    printerStatus->toJson(json);
    networkConfig->toJson(json);

    String jsonString;
    serializeJson(json, jsonString);

    return jsonString;
}