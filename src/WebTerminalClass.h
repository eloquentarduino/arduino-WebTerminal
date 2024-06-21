#ifndef WEBTERMINALCLASS_H
#define WEBTERMINALCLASS_H

#include <Arduino.h>
#include "./defines.h"


class WebTerminalClass {
public:

    /**
     * Setup web terminal
     * @param ssid
     * @param password
     * @param clientUUID
     * @return
     */
    bool begin(String ssid, String password, String clientUUID);

    /**
     *
     * @param fmt
     * @param ...
     */
    void queue(const char *fmt, ...);
};

static WebTerminalClass WebTerminal;

#endif //WEBTERMINALCLASS_H
