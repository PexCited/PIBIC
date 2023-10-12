#include "connection/network.h"
#include "modules/ota.h"

void setup(){
    Serial.begin(115200);
    config_networking();
    setup_ota();
    WiFi.mode(WIFI_AP);
    WiFi.softAP("esp32");
}

void loop(){
    ArduinoOTA.handle();
}