#include "HX711.h"

#define DOUT1  23
#define DOUT2  22
#define DOUT3  18
#define DOUT4  17
#define CLK   19

HX711 peso1;
HX711 peso2;
HX711 peso3;
HX711 peso4;

long temporizador;
int incoming;
void calibrate(HX711 myScale);

bool calibrating = false;
int calibration_step = 0;
long weight_reference = 0;

void setup() {
  Serial.begin(115200);
  peso1.begin(DOUT1, CLK);
  peso2.begin(DOUT2, CLK);
  peso3.begin(DOUT3, CLK);
  peso4.begin(DOUT4, CLK);
}

void loop() {
  if (calibrating) {
    if (calibration_step == 1) {
      Serial.println("Etapa 1: Retire o peso e pressione Enter");
      while (Serial.available()) Serial.read();
      while (Serial.available() == 0);
      calibration_step = 2;
    } else if (calibration_step == 2) {
      Serial.println("Etapa 2: Coloque o peso de referência em (g) e pressione Enter");
      while (Serial.available()) Serial.read();
      if (Serial.peek() != '\n') {
        if (Serial.available()) {
          char ch = Serial.read();
          if (isdigit(ch)) {
            weight_reference *= 10;
            weight_reference += (ch - '0');
          }
        }
      } else {
        Serial.print("Peso de referência: ");
        Serial.println(weight_reference);
        calibrate(peso1, weight_reference);
        calibrate(peso2, weight_reference);
        calibrate(peso3, weight_reference);
        calibrate(peso4, weight_reference);
        Serial.println("Calibração concluída");
        calibration_step = 0;
        calibrating = false;
      }
    }
  } else {
    // O resto do loop principal do seu código aqui
  }
}

void calibrate(HX711 myScale, long referenceWeight) {
  myScale.tare(100000); // ou outro valor adequado
  uint32_t offset = myScale.get_offset();
  Serial.print("Offset: ");
  Serial.println(offset);
  myScale.set_scale(myScale.get_scale());
  myScale.set_offset(offset);
  Serial.print("Escala: ");
  Serial.println(myScale.get_scale());
}

