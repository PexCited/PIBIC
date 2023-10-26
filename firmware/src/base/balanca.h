#include "common.h"
#include "modules/ota.h"
#include "connection/MQTT.h"
#include "connection/network.h"
#include <Arduino.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include "carga.h"

#include <string>

using namespace std;
using nlohmann::json;


float contador = 0;

void callback(char *topic, byte *payload, unsigned int length);
void config_callback(byte *payload, unsigned int length);
typedef void (*callback_t)(byte*, unsigned int);

unordered_map<string, callback_t> callback_map = {
    {"config", &config_callback},
};


void callback(char *topic, byte *payload, unsigned int length) {
    char *token = strtok(topic, "/");
    string topic_str(token);
    auto callback_it = callback_map.find(topic_str);
    if(callback_it != callback_map.end()) {
        callback_t callback_fn = callback_it->second;
        callback_fn(payload, length);
    } else {
        Serial.print("Callback not found for topic: ");
        Serial.println(topic_str.c_str());
    }

}


void config_callback(byte *payload, unsigned int length) {
    json config = json::parse(payload, payload + length);
    calibrate(&peso1);
    calibrate(&peso2);
    calibrate(&peso3);
    calibrate(&peso4);
}

json make_payload(){
  json payload;

  float valor_B2 = analogRead(A0);

  payload["B2"] = valor_B2;

  payload["B1"] = peso1.get_units();
  //payload["B2"] = peso2.get_units();
  payload["B3"] = peso3.get_units();
  payload["B4"] = peso4.get_units();

  return payload;
}

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  config_networking();

  #ifdef ENABLE_OTA
    setup_ota();
  #endif

  #ifdef ENABLE_MQTT
    config_mqtt(callback);
  #endif

  peso1.begin(DOUT1, CLK);
  peso2.begin(DOUT2, CLK);
  peso3.begin(DOUT3, CLK);
  peso4.begin(DOUT4, CLK);
}

void loop() {
  #ifdef ENABLE_MQTT
    client.loop();
    if (!client.connected()) {
      Serial.println("FALHA DE COMUNICAÇÃO...");
      reconnect();
    }
  #endif
  #ifdef ENABLE_OTA
    ArduinoOTA.handle();
  #endif

  if(contador < 100){ // Eniva no maximo 100msg.
    client.publish(TOPIC_DATA, make_payload().dump().c_str());
    contador ++;
  }

  if((millis()-temporizador)>1000){ // A  cada 1s reseta o contador.
    temporizador = millis();
    contador = 0;
  }

}
