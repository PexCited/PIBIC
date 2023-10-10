#include "HX711.h"
#define DOUT1  23
#define DOUT2  22
#define DOUT3  18
#define DOUT4  17
#define CLK   19
#include "BluetoothSerial.h" 
BluetoothSerial ESP_BT; 

HX711 peso1;                // instancia Balança HX711
HX711 peso2;                // instancia Balança HX711
HX711 peso3;                // instancia Balança HX711
HX711 peso4;                // instancia Balança HX711

long temporizador;
int incoming;
void calibrate(HX711 myScale);

void setup()
{ 
  Serial.begin(115200);
  ESP_BT.begin("ESP32_Control");
  peso1.begin(DOUT1, CLK);
  peso2.begin(DOUT2, CLK);
  peso3.begin(DOUT3, CLK);
  peso4.begin(DOUT4, CLK);
  calibrate(&peso1);
  calibrate(&peso2);
  calibrate(&peso3);
  calibrate(&peso4);
  Serial.print("P1,");
  Serial.print("P2,");
  Serial.print("P3,");
  Serial.println("P4;");
}

void loop(){
  float p1 = peso1.get_units();
  float p2 = peso2.get_units();
  float p3 = peso3.get_units();
  float p4 = peso4.get_units();

  if (ESP_BT.available()){
    incoming = ESP_BT.read();
    int button = floor(incoming / 10);
    int value = incoming % 10;
    switch (button) {
      case 1:  
        Serial.print("Button 1:"); Serial.println(value);
        // digitalWrite(led_pin_1, value);
        break;
      case 2:  
        Serial.print("Button 2:"); Serial.println(value);
        // digitalWrite(led_pin_2, value);
        break;
      case 3:  
        Serial.print("Button 3:"); Serial.println(value);
        // digitalWrite(led_pin_3, value);
        break;
    }
  } else {
    Serial.print(p1);     
    Serial.print(",");
    Serial.print(p2);
    Serial.print(",");
    Serial.print(p3);
    Serial.print(",");
    Serial.print(p4);
    Serial.println(";");
    // if((millis() - temporizador)>100){

    //   temporizador = millis();
    //   ESP_BT.print(p1, DEC);
    //   ESP_BT.print("|");
    //   ESP_BT.print(p2, DEC);
    //   ESP_BT.print("|");
    //   ESP_BT.print(p3, DEC);
    //   ESP_BT.print("|");
    //   ESP_BT.print(p4, DEC);
    //   ESP_BT.print("|");
    // }
  }

}

void calibrate(HX711 *myScale){
  while (Serial.available()) Serial.read();   /// Limpra qualquer coisa na serial;
  Serial.println("Retire o peso e aperte Enter");
  while (Serial.available() == 0);
  Serial.println("Fazendo a Tara");
  myScale->tare(100000*10000);// nao aumentar  o numero pode variar muito os pontos flutuantes 
  uint32_t offset = myScale->get_offset();
  Serial.println("Coloque o peso em (g) e aperte Enter");
  while (Serial.available()) Serial.read();   /// Limpra qualquer coisa na serial;
  uint32_t weight = 0;
  while (Serial.peek() != '\n')
  {
    if (Serial.available())
    {
      char ch = Serial.read();
      if (isdigit(ch))
      {
        weight *= 10;
        weight = weight + (ch - '0');
      }
    }
  }
  Serial.print("Peso: ");
  Serial.println(weight);
  // uint32_t weight = 10000; pesso sempre como referencia de carga para  porem colocar variavel 
  myScale->calibrate_scale(weight, 20);
  float escala = myScale->get_scale();
  myScale->set_scale(escala);
  myScale->set_offset(offset);
  Serial.print("Offset: ");
  Serial.print(offset);
  Serial.print("Escala: ");
  Serial.println(escala);
}