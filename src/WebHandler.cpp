//
// Created by timothy on 3/2/20.
//

#include "WebHandler.h"
#include "Data.h"
#include <sstream>

WebSocketsServer wss = WebSocketsServer(WS_PORT);
ESP8266WebServer web(WEB_PORT);
DNSServer dns = DNSServer();
int connected = 0;
int pushCounter = 0;

void WebHandler::init() {

    IPAddress ip(IP_1, IP_2, IP_3, IP_4);

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
    WiFi.softAP(SSID, PASS);


    dns.start(DNS_PORT, "*", ip);

    SPIFFS.begin();

    web.onNotFound([]() {
        if(!handleFileRead(web.uri())){
            const char *metaRefreshStr = R"(<head><meta http-equiv="refresh" content="0; url=http://192.168.1.1/index.html" /></head><body><p>redirecting...</p></body>)";
            web.send(200, "text/html", metaRefreshStr);
        }
    });

    web.begin();

    wss.begin();
    wss.onEvent(webSocketEvent);
}

void WebHandler::deinit() {
    SPIFFS.end();
}

void WebHandler::tick(Data* data)
{
    wss.loop();
    dns.processNextRequest();
    web.handleClient();
    Serial.println(ESP.getFreeHeap());
    push(data);
}

void WebHandler::push(Data* m_data) {
    if (connected && pushCounter % PUSH_EVERY_TICKS == 0) { //if there are any clients connected;
        std::ostringstream json;
        json << "{\"ax\":" << m_data->axial_accel.x << ", ";
        json << "\"ay\":" << m_data->axial_accel.y << ", ";
        json << "\"az\":" << m_data->axial_accel.z << ", ";
        json << "\"rx\":" << m_data->radial_accel.x << ", ";
        json << "\"ry\":" << m_data->radial_accel.y << ", ";
        json << "\"rz\":" << m_data->radial_accel.z << ", ";
        json << "\"av\":" << m_data->angularVelocity << ", ";
        json << "\"log\":" << (m_data->logging ? "true" : "false") << ", ";
        json << "\"naa\":" << m_data->netLinearAcc << "}";
        wss.broadcastTXT(json.str().c_str());
        if (pushCounter == PUSH_EVERY_TICKS) pushCounter = 0;
    }
    pushCounter++;
}

void WebHandler::webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            connected--;
            break;
        case WStype_CONNECTED:
            connected++;
//             send message to client
            wss.sendTXT(num, "hi");
            break;
        default:
            break;
    }
}

bool WebHandler::handleFileRead(String path) {
    Serial.println("handleFileRead: " + path);
    if(path.endsWith("/")) path += "index.html";
    String contentType = getContentType(path);
    String pathWithGz = path + ".gz";
    if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
        if(SPIFFS.exists(pathWithGz))
            path += ".gz";
        File file = SPIFFS.open(path, "r");
        web.streamFile(file, contentType);
        file.close();
        return true;
    }
    return false;}

String WebHandler::getContentType(String filename) {
    if(web.hasArg("download")) return "application/octet-stream";
    else if(filename.endsWith(".htm")) return "text/html";
    else if(filename.endsWith(".html")) return "text/html";
    else if(filename.endsWith(".css")) return "text/css";
    else if(filename.endsWith(".js")) return "application/javascript";
    else if(filename.endsWith(".png")) return "image/png";
    else if(filename.endsWith(".gif")) return "image/gif";
    else if(filename.endsWith(".jpg")) return "image/jpeg";
    else if(filename.endsWith(".ico")) return "image/x-icon";
    else if(filename.endsWith(".xml")) return "text/xml";
    else if(filename.endsWith(".pdf")) return "application/x-pdf";
    else if(filename.endsWith(".zip")) return "application/x-zip";
    else if(filename.endsWith(".gz")) return "application/x-gzip";
    return "text/plain";
}

