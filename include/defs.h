#ifndef DEFS_H
#define DEFS_H

#define FIRMWARE_VERSION "1.0.0"

/**
 * Access point
 */
#define AP_SSID "Bambu Controller"
#define AP_PASSWORD "bambu123"

/**
 * General pins
 */
#define CEILING_LED_PIN 21

/**
 * Led Strip pins
 */
#define LED_STRIP_RED_PIN 19
#define LED_STRIP_GREEN_PIN 18
#define LED_STRIP_BLUE_PIN 21
#define LED_STRIP_YELLOW_PIN 22
#define LED_STRIP_WHITE_PIN 23

/**
 * Bambu MQTT buffer
 */
#define MAX_BUFFER_SIZE 32768
#define MAX_DOCUMENT_SIZE MAX_BUFFER_SIZE / 2

/**
 * Webserver HTTP_POST body max size (when sending JSON)
 */
#define MAX_HTTP_POST_JSON_BODY_SIZE 512U

/**
 * Filesystem related
 */
#define FS_MAX_JSON_SIZE 1024

#endif