#include "connection/network.h"
#include "modules/ota.h"

void setup(){
    Serial.begin(115200);
    // config_networking();
    WiFi.mode(WIFI_AP);
    WiFi.softAP("esp32");
    setup_ota();
}

void loop(){
    ArduinoOTA.handle();
}