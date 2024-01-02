#ifndef EVENTEMITTER_H
#define EVENTEMITTER_H

#include <Arduino.h>
#include <unordered_map>

// Event emitter class with key-based events
template <typename... Args>
class EventEmitter
{
public:
    using Callback = std::function<void(Args...)>;

    // Subscribe to an event with a specific key
    void on(const std::string &key, const Callback &callback)
    {
        callbacks[key].push_back(callback);
    }

    // Unsubscribe from an event with a specific key
    void off(const std::string &key, const Callback &callback)
    {
        auto it = callbacks.find(key);
        if (it != callbacks.end())
        {
            auto &keyCallbacks = it->second;
            auto callbackIt = std::find(keyCallbacks.begin(), keyCallbacks.end(), callback);
            if (callbackIt != keyCallbacks.end())
            {
                keyCallbacks.erase(callbackIt);
            }
        }
    }

    // Emit an event with a specific key and variable arguments
    void emit(const std::string &key, Args... args)
    {
        auto it = callbacks.find(key);
        if (it != callbacks.end())
        {
            for (const auto &callback : it->second)
            {
                callback(args...);
            }
        }
    }

private:
    std::unordered_map<std::string, std::vector<Callback>> callbacks;
};

#endif