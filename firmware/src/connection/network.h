#ifndef NETWORKING_H
    #define NETWORKING_H
    #include "base/common.h"

    #if defined(ENABLE_NET) || defined(ENABLE_DHCP) || defined(ENABLE_WIFI)
        #ifdef ENABLE_WIFI
            #include "connection/WIFI.h"
        #else
            #include "connection/RJ45.h"
        #endif

        void config_networking(){
            #ifdef ENABLE_WIFI
                config_wifi();
            #else
                config_ETH();
            #endif
        }
    #else
        void config_networking(){
            Serial.println(" ENABLE_NET OR ENABLE_DHCP OR ENABLE_WIFI MUST BE SET ON BUILDFALGS");
        }
    #endif
#endif
