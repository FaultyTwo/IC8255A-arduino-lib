/* WriteData
 * Author: Chawin 'FaultyTwo' Treesugol
 * Date: 5/6/2022
 * FROM: https://github.com/FaultyTwo/TC74-arduino-lib
 * 
 * Desc: This sketch allows Arduino to control 8255A
 * through UART communication by sending in data like this:
 * "*port**value*"
 * Example. A08 -> Send value 8 to port A
 * B55 -> Send value 55 to port B
 * C128 -> Send value 128 to port C
 * .etc .etc
 * 
 * Or just bring up Serial Monitor and type in.
 * 
 * Enjoy!
 */
 
#include "8255A.h"

int data[8] = {2,3,4,5,6,7,8,9};
int reg[4] = {12,13,11,10};

IC_8255A ppi(data,reg);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ppi.pin_config(0x00); // setting all pins to read
  ppi.mode_config(0x80); // setting all ports to write
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    String data = Serial.readStringUntil('\n');
    String sub;
    if(data.startsWith("a") || data.startsWith("A")){
      sub = data.substring(1);
      ppi.write('a',sub.toInt());
    }

    if(data.startsWith("b") || data.startsWith("B")){
      sub = data.substring(1);
      ppi.write('b',sub.toInt());
    }

    if(data.startsWith("c") || data.startsWith("C")){
      sub = data.substring(1);
      ppi.write('c',sub.toInt());
    }
  }
}
