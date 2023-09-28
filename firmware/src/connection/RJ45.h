#ifndef RJ45_H
    #define RJ45_H
    #include "base/common.h"
    // usigned long 
    void config_ETH(){

        #ifdef OPS0_h
            if (SS != 15){
                Serial.println("Usando oppler é necessário editar a definição dos pinos padrões do SPI (SS, CLK, MISO, MOSI) para (15, 14, 12, 13)");
                Serial.println("Build pelo platformio o arquivo pode ser encontrado em 'C:/Users/%USER%/.platformio/packages/framework-arduinoespressif32/variants/esp32/pins_arduino.h'");
                Serial.println(
                    "static const uint8_t SS    = 15; //5;\n"
                    "static const uint8_t MOSI  = 13; //23;\n"
                    "static const uint8_t MISO  = 12; //19;\n"
                    "static const uint8_t SCK   = 14; //18;\n"
                );
            }
        #endif

        Ethernet.init(SS);
        if (Ethernet.hardwareStatus() == EthernetNoHardware) {
            Serial.println(
                    "Ethernet shield was not found.  Sorry, can't run without hardware. :(");
            while (true) {
                Serial.println("Rebooting...");
                delay(5000); // do nothing, no point running without Ethernet hardware
                ESP.restart();
            }
        }else{
            if (Ethernet.linkStatus() == LinkOFF) {
                Serial.println("Ethernet cable is not connected.");
            }

            #ifdef ENABLE_DHCP
                if (Ethernet.begin(_ESP_MAC)) { // Dynamic IP setup
                    Serial.println("DHCP OK!");
                }else{
                    Serial.println("Failed to configure Ethernet using DHCP");
                }
            #else
                Ethernet.begin(_ESP_MAC, _ESP_IP, _ESP_DNS, _ESP_GW, _ESP_MASK);
            #endif

        }
            Serial.print("Local IP : ");
            Serial.println(Ethernet.localIP());
            Serial.print("Subnet Mask : ");
            Serial.println(Ethernet.subnetMask());
            Serial.print("Gateway IP : ");
            Serial.println(Ethernet.gatewayIP());
            Serial.print("DNS Server : ");
            Serial.println(Ethernet.dnsServerIP());
        
            Serial.println("Ethernet Successfully Initialized");
        }
#endif