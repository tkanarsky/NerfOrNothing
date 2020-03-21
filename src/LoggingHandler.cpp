//
// Created by timothy on 3/3/20.
//

#include "LoggingHandler.h"
#include <sstream>

bool LoggingHandler::lastButtonState = false;
bool LoggingHandler::latestSettledState = false;
time_t LoggingHandler::debounce_t = 0;
File LoggingHandler::currFile = File();
FtpServer ftp = FtpServer();

void LoggingHandler::init() {
    debounce_t = millis();
    Serial.println("About to initialize ftp server...");
    Serial.println("Initialized ftp server...");
    ftp.begin(FTP_NAME, FTP_PASS);
    Serial.println("Launched ftp server!");
    pinMode(LOG_BUTTON_PIN, INPUT);
    pinMode(BEEPER_PIN, OUTPUT);
    Serial.println("Done with logging handler init!");
}

void LoggingHandler::deinit() {
    SPIFFS.end();
}

void LoggingHandler::tick(Data* m_data) {
    if (m_data->logging && currFile) {
        std::ostringstream line;
        line << millis() << ",";
        line << m_data->radial_accel_mapped.x << ",";
        line << m_data->radial_accel_mapped.y << ",";
        line << m_data->radial_accel_mapped.z << ",";
        line << m_data->axial_accel_mapped.x << ",";
        line << m_data->axial_accel_mapped.y << ",";
        line << m_data->axial_accel_mapped.z << "\n";
        currFile.write(line.str().c_str());
    }
    bool current = digitalRead(LOG_BUTTON_PIN);
    if (current != lastButtonState) debounce_t = millis();
    if (millis() - debounce_t > 20) {
        if (current != latestSettledState) {
            latestSettledState = current;
            if (!current) {
                m_data->logging = !m_data->logging;
                if (m_data->logging) { // we just started logging
                    std::ostringstream path;
                    path << "/" << millis() << ".csv";
                    currFile = SPIFFS.open(path.str().c_str(), "w");
                    tone(BEEPER_PIN, 880, 100);
                    delay(50);
                    tone(BEEPER_PIN, 1174, 200);
                } else { // we just finished logging
                    currFile.close();
                    tone(BEEPER_PIN, 1174, 100);
                    delay(50);
                    tone(BEEPER_PIN, 880, 200);
                }
            }
        }
    }
    ftp.handleFTP();
    lastButtonState = current;
}
