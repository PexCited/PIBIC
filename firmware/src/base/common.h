#ifndef COMMON_H
    #define COMMON_H
    #include <Ethernet.h>
    #include <PubSubClient.h>
    #ifdef ENABLE_WIFI
        #include <WiFiManager.h>
        WiFiClient espClient;
    #else
        EthernetClient espClient;
    #endif

    PubSubClient client(espClient);

    #define SERIAL_BAUDRATE 115200

    #define RST_PIN 2
    #define SPI_HOST       1
    #define SPI_CLOCK_MHZ  8
    #define INT_GPIO       4
    
    #include "custom.h"
#endif