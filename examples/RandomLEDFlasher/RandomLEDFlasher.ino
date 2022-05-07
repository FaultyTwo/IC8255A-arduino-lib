/* RandomLEDFlasher
 * Author: Chawin 'FaultyTwo' Treesugol
 * Date: 5/5/2022
 * FROM: https://github.com/FaultyTwo/LTC6904-arduino-lib
 * 
 * Desc:
 * This sketch turns any led array devices into a flasher
 * Ex. Dot Matrix, Bargraph, or.. just plain 8 or 16 LEDs
 * 
 * Enjoy!
 */

#include "8255A.h"

int data[8] = {2,3,4,5,6,7,8,9};
int reg[4] = {12,13,11,10};

uint8_t ranNum, ranNum2; //one byte

IC_8255A ppi(data,reg);

void setup() {
  // put your setup code here, to run once:
  ppi.pin_config(0x00);
  ppi.mode_config(0x80);
  randomSeed(analogRead(0));
}

//me when spicy chip
void loop() {
  // put your main code here, to run repeatedly:
  ranNum = random(255);
  ranNum2 = random(255);
  ppi.write('C',ranNum); //
  ppi.write('B',ranNum2); //
  delay(50);
}
