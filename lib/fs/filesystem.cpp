#include <filesystem.h>

FileSystem::FileSystem(NetworkConfig *netConfig, PrinterConfig *printer, SystemConfig *bcConfig)
    : filePath("/config.json"),
      networkConfig(netConfig),
      printerConfig(printer),
      systemConfig(bcConfig)
{
}

bool FileSystem::save()
{
    Serial.println("Saving to filesystem...");

    DynamicJsonDocument json = writeJson();
    File file = LittleFS.open(filePath, "w", true);

    if (!file)
    {
        Serial.println("Filesystem saving failed");
        return false;
    }

    serializeJson(json, file);
    file.close();
    Serial.println("Filesystem saved!");
    return true;
}

bool FileSystem::load()
{
    Serial.println("Loading filesystem");

    File file = LittleFS.open(filePath, "r");

    if (!file)
    {
        Serial.println("Failed to open config file");
        return false;
    }

    size_t size = file.size();

    if (size == 0)
        return false;

    std::unique_ptr<char[]> buf(new char[size]);
    file.readBytes(buf.get(), size);

    DynamicJsonDocument json(FS_MAX_JSON_SIZE);

    auto deserializeError = deserializeJson(json, buf.get());

    if (deserializeError)
    {
        Serial.println("Failed loading config");
        Serial.println("Clearing config");
        file.close();
        LittleFS.remove(filePath);
        return false;
    }

    readJson(json);
    file.close();
    return true;
}

void FileSystem::erase()
{
    Serial.println("Erasing filesystem...");
    LittleFS.remove(filePath);
}

bool FileSystem::mount()
{
    Serial.println("Mounting filesystem...");
    if (!LittleFS.begin(true))
    {
        Serial.println("Failed to mount filesystem!");
        return false;
    }
    Serial.println("Filesystem mounted!");
    return true;
}

void FileSystem::readJson(DynamicJsonDocument &json)
{
    systemConfig->fromJson(json);
    printerConfig->fromJson(json);
    networkConfig->fromJson(json);
    Serial.println("Loaded config");
}

DynamicJsonDocument FileSystem::writeJson()
{
    DynamicJsonDocument json(FS_MAX_JSON_SIZE);
    systemConfig->toJson(json);
    printerConfig->toJson(json, true);
    networkConfig->toJson(json);
    return json;
}

void FileSystem::update(DynamicJsonDocument json)
{
    readJson(json);
    save();
}