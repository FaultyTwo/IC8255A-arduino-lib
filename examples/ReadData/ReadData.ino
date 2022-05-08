/* ReadData
 * Author: Chawin 'FaultyTwo' Treesugol
 * Date: 5/5/2022
 * FROM: https://github.com/FaultyTwo/TC74-arduino-lib
 * 
 * Desc: An example usage of read functionality of 8255A
 * In this sketch. Pin A0 is connected to a button or a trigger.
 * When A0 goes HIGH, it reads the data from port A and display
 * the value in Serial Monitor.
 * 
 * Enjoy!
 */
 
#include "8255A.h"

int data[8] = {2,3,4,5,6,7,8,9};
int reg[4] = {12,13,11,10};

IC_8255A ppi(data,reg);
bool reading = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ppi.pin_config(0xFF); // setting all pins to read
  ppi.mode_config(0x90); // reading from port a
  pinMode(A0,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(A0) == HIGH){
    if(reading == false){
      Serial.println(ppi.read('a'));
      reading = true;
    }
  } else {
    reading = false;
  }

}
