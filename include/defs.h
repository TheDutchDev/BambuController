#ifndef DEFS_H
#define DEFS_H

#define FIRMWARE_VERSION "1.0.0"

/**
 * General pins
 */
#define CEILING_LED_PIN 21

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