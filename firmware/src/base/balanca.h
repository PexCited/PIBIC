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

#define CONFIRM_PIN 4
#define CANCEL_PIN  5

bool _CONFIRM = false;
bool _CANCEL = false;
int step = 0;
long button = millis();

// string operator_name = DEFAULT_OP;
// string modelo = DEFAULT_MD;

string status_code="ok";
bool CONFIG_COMPLETED = false;
bool STOP_NOW = false;
bool UPDATE = false;

// void IRAM_ATTR CONFIRM_ACTION();
// void IRAM_ATTR CANCEL_ACTION();
void callback(char *topic, byte *payload, unsigned int length) {}

json make_payload(){
  json payload;

  payload["value"] = peso1.get_units();

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

  // attachInterrupt(CONFIRM_PIN, CONFIRM_ACTION, CHANGE);
  // attachInterrupt(CANCEL_PIN, CANCEL_ACTION, CHANGE);

  pinMode(CONFIRM_PIN, INPUT_PULLUP);
  pinMode(CANCEL_PIN, INPUT_PULLUP);

  peso1.begin(DOUT1, CLK);
  calibrate(&peso1);
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


  if((millis()-temporizador)>1000){
    client.publish(TOPIC_DATA, make_payload().dump().c_str());
    temporizador = millis();
  }

}
