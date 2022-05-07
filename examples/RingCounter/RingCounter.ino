/* RingCounter
 * Author: Chawin 'FaultyTwo' Treesugol
 * Date: 5/5/2022
 * FROM: https://github.com/FaultyTwo/TC74-arduino-lib
 * 
 * Desc: This is a silly example for 8255A library's usage.
 * This sketch turns 8255A into a johnson's ring counter.
 * 
 * This also can be used to test every pins of each 8255A ports too.
 * 
 * Enjoy!
 */

#include "8255A.h"

int data[8] = {2,3,4,5,6,7,8,9};
int reg[4] = {12,13,11,10};

int shift = 0x01;
int count = 0;
char port[] = {'A','B','C'};

IC_8255A ppi(data,reg);

void setup() {
  // put your setup code here, to run once:
  ppi.pin_config(0x00); // 0 = write, 1 = read
  ppi.mode_config(0x80); // setting all 8255a ports to write
}

void loop() {
  // put your main code here, to run repeatedly:
  int prev = 0x00;
  for(int i = 0; i < 24; i++){
      switch(i){
        case 8:
        case 16:
          count++;
          shift = 0x01;
          prev = 0;
      }
      ppi.write(port[count],shift | prev);
      prev |= shift;
      shift <<= 1;
      delay(50);
  }
  shift = 0xFF;
  count = 0;
  for(int i = 0; i < 25; i++){
      switch(i){
        case 8:
        case 16:
          ppi.write(port[count],0x00);
          count++;
          shift = 0xFF;
          break;
      }
      ppi.write(port[count],shift);
      shift <<= 1;
      delay(50);
  }
  ppi.write(port[count],0x00);
  delay(50);
  shift = 0x01;
  count = 0;
}
