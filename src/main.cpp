//
// Created by timothy on 2/22/20.
//


#include <Arduino.h>

#include "Data.h"
#include "WebHandler.h"
#include "AccelHandler.h"
#include "LoggingHandler.h"

Data data;

void setup() {
    Serial.begin(115200);
    delay(10);
    AccelHandler::init();
    Serial.println("Initialized accel handler!");
    WebHandler::init();
    Serial.println("Initialized web handler!");
    LoggingHandler::init();
    Serial.println("Initialized logging handler!");
}


void loop() {
    AccelHandler::tick(&data);
    WebHandler::tick(&data);
    LoggingHandler::tick(&data);
    delay(10);
}