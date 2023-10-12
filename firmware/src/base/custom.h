#ifndef CUSTOM_H
    #define CUSTOM_H
    #include <Ethernet.h>

    #ifndef MODULE_ID
        #define MODULE_ID  "1"
    #endif

    #ifndef MODULE_TYPE
        #define MODULE_TYPE "GENERIC"
    #endif

    #ifndef ENABLE_DCHP
        IPAddress _ESP_IP(192, 168, 18, 235);    // *** CHANGE THIS to something relevant for YOUR LAN,  ***
        IPAddress _ESP_MASK(255, 255, 255, 0);   // Subnet mask, 
        IPAddress _ESP_DNS(192, 168, 18, 1);    // *** CHANGE THIS to match YOUR DNS server,            ***
        IPAddress _ESP_GW(192, 168, 18, 1);   // *** CHANGE THIS to match YOUR Gateway (router).     ***
    #endif
    uint8_t   _ESP_MAC[] = { 0x02, 0xF0, 0x0D, 0xBE, 0xEF, 0x50 };


    #define TOPIC_BASE MODULE_TYPE "/" MODULE_ID
    #define TOPIC_ALERT "alert/" TOPIC_BASE 
    #define TOPIC_DATA "data/" TOPIC_BASE 
    #define TOPIC_CONFIG "config/" TOPIC_BASE
    #define TOPIC_STATUS "action/" TOPIC_BASE
    #define TOPIC_SMS "sms/" TOPIC_BASE
    #define TOPIC_INFO "info/" TOPIC_BASE

#endif