#ifndef WIFIM_H
    #define WIFIM_H
    #define WM_TITLE "Notify WiFi Config"
    #define WM_CLASS "invert"
    #define WM_PASSWORD "notify12345"
    #define WM_PROTAL_NAME "MODULE " MODULE_TYPE " | " MODULE_ID
    #include "base/common.h"

    WiFiManager wm;
    WiFiManagerParameter custom_field;

    String MAC_ADDRESS;

    void resetToFactoryDefaults() {
        wm.resetSettings();
    }

    bool config_wifi()
    {
        #ifdef RESET_WIFI
            resetToFactoryDefaults();
        #endif

        MAC_ADDRESS = WiFi.macAddress();

        WiFi.mode(WIFI_STA);
        wm.setConnectTimeout(320);
        wm.setCleanConnect(true);
        wm.setAPStaticIPConfig(IPAddress(192, 168, 5, 1), IPAddress(192, 168, 5, 1), IPAddress(255, 255, 255, 0));
        wm.setRemoveDuplicateAPs(true); 
        wm.setWiFiAutoReconnect(true);
        wm.setAPClientCheck(true);

        Serial.setDebugOutput(true);
        delay(3000);
        pinMode(RST_PIN, INPUT_PULLDOWN);
        wm.setTitle(WM_TITLE);
        wm.setClass(WM_CLASS);
        wm.setConfigPortalTimeout(120);
        #ifndef ENABLE_DCHP
            wm.setSTAStaticIPConfig(_ESP_IP, _ESP_GW, _ESP_MASK, _ESP_DNS);
        #endif
        return wm.autoConnect(WM_PROTAL_NAME, WM_PASSWORD);
    }
    
#endif