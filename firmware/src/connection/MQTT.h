#ifndef MQTT_H
    #define MQTT_H

    #ifndef MQTT_PORT
        #define MQTT_PORT 1883
    #endif
    #ifndef MQTT_MAX_PACKET_SIZE
        #define MQTT_MAX_PACKET_SIZE 512;
    #endif
    
    #ifndef MQTT_HOST
        #define MQTT_HOST '192.168.10.100'
    #endif

    #ifdef ENABLE_OTA
        #include "modules/ota.h"
    #endif

    #include "base/common.h"
    void config_mqtt(void (*func)(char *, uint8_t *, unsigned int)){
            client.setServer(MQTT_HOST, MQTT_PORT);
            client.setCallback(func);
            client.setBufferSize(MQTT_MAX_PACKET_SIZE);
    }
    
    void intToConstChar(unsigned int num, char* buffer, size_t bufferSize) {
        snprintf(buffer, bufferSize, "%u", num);
    }

    void reconnect() {
        unsigned int randomNum = esp_random();

        // Definir um tamanho para o buffer que vai conter a string convertida
        const int bufferSize = 7;
        char buffer[bufferSize];

        // Converter o número aleatório para uma const char*
        intToConstChar(randomNum, buffer, bufferSize);

        // Agora, o buffer contém a string convertida e pode ser usada para montar a string aleatória

        client.setKeepAlive(6);
        while (!client.connected()) {
            #ifdef ENABLE_OTA
                ArduinoOTA.handle();
            #endif
            Serial.print("Attempting MQTT connection...");
            if (client.connect(buffer)) {
            Serial.println("connected");
            } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
            }
        }
    }
#endif
