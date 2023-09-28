#include "common.h"
#include "modules/ota.h"
#include "connection/MQTT.h"
#include "connection/network.h"
#include <Arduino.h>
#include <iostream>
#include <nlohmann/json.hpp>

#include <string>

using namespace std;
using nlohmann::json;

#define CONFIRM_PIN 4
#define CANCEL_PIN  5

bool _CONFIRM = false;
bool _CANCEL = false;
int step = 0;
long button = millis();

string operator_name = DEFAULT_OP;
string modelo = DEFAULT_MD;

string status_code="ok";
bool CONFIG_COMPLETED = false;
bool STOP_NOW = false;
bool UPDATE = false;

void IRAM_ATTR CONFIRM_ACTION();
void IRAM_ATTR CANCEL_ACTION();
void callback(char *topic, byte *payload, unsigned int length) {}

json make_payload(){
  json payload;

  payload["operator"] = operator_name != DEFAULT_OP? operator_name : "" ;
  payload["modelo"] = modelo != DEFAULT_MD? modelo : "";
  payload["type"] = MODEL;

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

  attachInterrupt(CONFIRM_PIN, CONFIRM_ACTION, CHANGE);
  attachInterrupt(CANCEL_PIN, CANCEL_ACTION, CHANGE);

  pinMode(CONFIRM_PIN, INPUT_PULLUP);
  pinMode(CANCEL_PIN, INPUT_PULLUP);
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


  if(UPDATE){
    UPDATE = false;
    if (CONFIG_COMPLETED || STOP_NOW ){
        //mount payload
        Serial.println("=-=-=-=-=-=-=-=-=-=-=-=");
        Serial.println(operator_name.c_str());
        Serial.println(modelo.c_str());
        Serial.println("=-=-=-=-=-=-=-=-=-=-=-=");
        client.publish(TOPIC_CONFIG, make_payload().dump().c_str());
        STOP_NOW = false;
    }
  }

}
