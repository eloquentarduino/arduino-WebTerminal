#include <cstdio>
#include <WiFi.h>
#include <MQTT.h>
#include "WebTerminalClass.h"


namespace {
    typedef struct {
        char text[WEBTERMINAL_BUFFER_SIZE];
    } message_t;

    const char *TAG = "WebTerminal";
    String topic;
    String clientID;
    message_t buffer;
    message_t payload;
    QueueHandle_t messageQueue;
    WiFiClient wifi;
    MQTTClient mqtt(WEBTERMINAL_BUFFER_SIZE);


    /**
     * Test if elapsed the given offset from last check.
     * Overflow safe.
     * @param now
     * @param last
     * @param offset
     * @return
     */
    bool elapsed(size_t now, size_t last, size_t offset) {
        return now < last || now > last + offset;
    }

    /**
     *
     * @return
     */
    bool connectToWifi(String ssid, String pass) {
        raw_log_i("[%s] Connecting to WiFi %s...", TAG, ssid.c_str());
        WiFi.mode(WIFI_STA);
        WiFi.disconnect(true, true);
        WiFi.begin(ssid, pass);

        const size_t timeout = millis() + 10000;

        while (millis() < timeout && WiFi.status() != WL_CONNECTED) {
            delay(250);
        }

        if (WiFi.status() == WL_CONNECTED) {
            raw_log_i("[%s] Connected to WiFi", TAG);
            return true;
        }

        if (WiFi.status() == 1) {
            raw_log_e("[%s] WiFi network %s not found", TAG, ssid.c_str());
            return false;
        }

        raw_log_e("[%s] Can't connect to WiFi", TAG);
        return false;
    }
}

/**
 *
 * @param ssid
 * @param password
 * @param clientUUID
 * @return
 */
bool WebTerminalClass::begin(String ssid, String password, String clientUUID) {
    clientID = clientUUID;

    messageQueue = xQueueCreate(WEBTERMINAL_QUEUE_SIZE, sizeof(message_t));

    // 1. connect to WiFI
    if (WiFi.status() != WL_CONNECTED)
        connectToWifi(ssid, password);

    if (WiFi.status() != WL_CONNECTED)
        return false;

    // 2. connect to MQTT
    mqtt.begin("mqtt.eloquentarduino.com", 1883, wifi);

    if (!mqtt.connect(clientUUID.c_str(), clientUUID.c_str(), "password")) {
        raw_log_e("[%s] Can't connect to MQTT broker. Are you sure your CLIENT_ID is valid?", TAG);
        return false;
    }

    topic = String("/eqrdnwtc-") + clientUUID;

    xTaskCreate([](void *args) {
        size_t heartbeatTick;
        bool connected = wifi.connected();

        while (true) {
            size_t now = millis();

            // await for queue
            if (messageQueue == NULL) {
                delay(10);
                continue;
            }

            // await for WiFi
            if (elapsed(now, heartbeatTick, 5000)) {
                connected = mqtt.connected() || mqtt.connect(clientID.c_str());
                heartbeatTick = millis();

                if (!connected)
                    raw_log_e("[%s] MQTT not connected", TAG);
            }

            if (!connected) {
                delay(10);
                continue;
            }

            // await for message
            if (xQueueReceive(messageQueue, &payload, 10) != pdPASS) {
                delay(10);
                continue;
            }

            // try to send N times
            for (uint8_t i = 0; i < 3; i++) {
                if (mqtt.publish(topic.c_str(), payload.text))
                    break;

                delay(1000);
            }
        }
    }, "WebTerminal", 50000, NULL, 0, NULL);

    return true;
}

/**
 *
 * @param fmt
 * @param ...
 */
void WebTerminalClass::queue(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsprintf(buffer.text, fmt, args);
    va_end(args);
    log_printf("%s", buffer.text);

    if (messageQueue == NULL) {
        raw_log_w("[%s] Message queue does not exist yet, log will be discarded: %s", TAG, buffer.text);
        return;
    }

    if (uxQueueSpacesAvailable(messageQueue) == 0) {
        raw_log_w("[%s] Message queue is full, log will be discarded: %s", TAG, buffer.text);
        return;
    }

    if (xQueueSend(messageQueue, (void *) &buffer, 0) != pdTRUE) {
        raw_log_w("[%s] Can't queue log: %s", TAG, buffer.text);
        return;
    }
}