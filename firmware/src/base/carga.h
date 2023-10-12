#include "HX711.h"
#define DOUT1  23
#define DOUT2  22
#define DOUT3  18
#define DOUT4  17
#define CLK   19

HX711 peso1; 

long temporizador;
int incoming;
void calibrate(HX711 myScale);

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