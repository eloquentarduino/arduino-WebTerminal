/**
 * Inspect logs from remote Serial Monitor
 *
 * Be sure to set "TOOLS > CORE DEBUG LEVEL = INFO"
 * to turn on debug messages
 *
 * Be sure to replace your WiFi credentials!
 */
#include <WebTerminal.h>

#define WIFI_SSID "SSID"
#define WIFI_PASS "PASSWORD"
#define CLIENT_ID "get yours at https://eloquentarduino.com/arduino-web-serial-monitor/"

/**
 *
 */
void setup() {
    delay(3000);
    Serial.begin(115200);
    Serial.println("___WEB TERMINAL EXAMPLE___");

    // connect to WiFi and remote terminal
    while (!WebTerminal.begin(WIFI_SSID, WIFI_PASS, CLIENT_ID)) {
        Serial.println("Can't connect to web terminal");
    }

    Serial.println("Connected to web terminal");
    Serial.print("Log messages will be visible online at ");
    Serial.print("https://eloquentarduino.com/");
    Serial.print("arduino-web-serial-monitor/");
    Serial.println(CLIENT_ID);
}

/**
 *
 */
void loop() {
    ESP_LOGI("APP", "This is a INFO message");
    ESP_LOGW("APP", "This is a WARN message");
    ESP_LOGE("APP", "This is a ERROR message");
    raw_log_i("This message will not be forwarded online");
    delay(3000);
}
