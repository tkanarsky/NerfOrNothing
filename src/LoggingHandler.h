//
// Created by timothy on 3/3/20.
//

#ifndef NERF_LOGGINGHANDLER_H
#define NERF_LOGGINGHANDLER_H
#include <FS.h>
#include "Data.h"
#include <ESP8266FtpServer.h>

#define LOG_BUTTON_PIN 15
#define BEEPER_PIN 14

#define FTP_NAME "Nerf"
#define FTP_PASS "nerfornothing"

class LoggingHandler {
public:
    LoggingHandler() = delete;
    static void init();
    static void deinit();
    static void tick(Data* data);
private:
    static File currFile;
    static bool logging;
    static bool lastButtonState;
    static bool latestSettledState;
    static time_t debounce_t;
};


#endif //NERF_LOGGINGHANDLER_H
