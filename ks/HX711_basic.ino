#include "HX711.h"
HX711 scale(A1, A0); // DOUT, SCK


void setup()
{
Serial.begin(9600);
scale.set_scale(2280.f); //// this value is obtained by calibrating the scale with known weights
scale.tare();
}

void loop()
{
Serial.print("one reading:\t");
Serial.print(scale.get_units(), 1);
scale.power_down(); // put the ADC in sleep mode
delay(5000);
scale.power_up();
}