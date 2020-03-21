//
// Created by timothy on 3/2/20.
//

#ifndef NERF_WEBHANDLER_H
#define NERF_WEBHANDLER_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

#include <FS.h>
#include <WebSocketsServer.h>
#include "Data.h"

#define DNS_PORT 53
#define WEB_PORT 80
#define WS_PORT 81

#define PUSH_EVERY_TICKS 5

#define IP_1 192
#define IP_2 168
#define IP_3 1
#define IP_4 1

#define SSID "Nerf Ball"
#define PASS "nerfornothing"

class WebHandler {
public:
    WebHandler() = delete;
    static void init();
    static void deinit();
    static void tick(Data* data);
    static String getContentType(String filename);
    static bool handleFileRead(String path);
    static void push(Data* data);
    static void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
};



#endif //NERF_WEBHANDLER_H
