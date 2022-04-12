#include "8255A.h"

// to save you from the insanity of putting each values into the constructor
// by allowing an array to be parsed instead of each values
// see? saving time and your insanity!
// and maybe you can delete those variable later

// function writing guide
// disable ~wr and ~rd first (both to '1') -> a0, a1 -> push in data -> operate the write or read

IC_8255A::IC_8255A(int data[], int spr[]){
  for(uint8_t i = 0; i < 8; i++){ //data i/o
    iodata[i] = data[i];
  }
  /* only need a0 a1 ~wr and ~wd 
    ~cs can go fuck itself
    spr[0] = a0
    spr[1] = a1
    spr[2] = ~wr
    spr[3] = ~rd
  */
  for(uint8_t i = 0; i < 4; i++){
    pinMode(spr[i],OUTPUT);
    iocontrol[i] = spr[i];
    if(i < 2) // normal pin
      digitalWrite(spr[i],LOW);
    else
      digitalWrite(spr[i],HIGH);
  }
}

void IC_8255A::mode_config(uint8_t word){
  // okay.. how am i going to make this work
  // disable read and write, config, then write

  digitalWrite(iocontrol[3],HIGH); //must config read bit every times
  digitalWrite(iocontrol[2],HIGH); 

  digitalWrite(iocontrol[0],HIGH);
  digitalWrite(iocontrol[1],HIGH);

  // okay.. now to turn word into bits
  for(uint8_t i = 0; i < 8; i++){
    digitalWrite(iodata[i],bitRead(word,i));
  }

  digitalWrite(iocontrol[2],LOW); //write it
}

void IC_8255A::pin_config(uint8_t data){
  // this is for configuring Arduino pins
  // not 8255A pins
  // 1 = read, 0 = write

  for(uint8_t i = 0; i < 8; i++){
    pinMode(iodata[i],bitRead(data,i) == 0 ? OUTPUT : INPUT);
  }

}

// write functions
void IC_8255A::write(char port, uint8_t data){
  // disable write and read port
  digitalWrite(iocontrol[3],HIGH);
  digitalWrite(iocontrol[2],HIGH); 

  // i think if-else is better than switch in this case
  if(port == 'A' || port == 'a'){
    digitalWrite(iocontrol[0],LOW);
    digitalWrite(iocontrol[1],LOW);
  } else if(port == 'B' || port == 'b') {
    digitalWrite(iocontrol[0],HIGH);
    digitalWrite(iocontrol[1],LOW);
  } else if(port == 'C' || port == 'c'){
    digitalWrite(iocontrol[0],LOW);
    digitalWrite(iocontrol[1],HIGH);
  } else {
    return; // port d.. doesn't exist
  }

  for(uint8_t i = 0; i < 8; i++){ //feed the data
    digitalWrite(iodata[i],bitRead(data,i));
  }

  digitalWrite(iocontrol[2],LOW); //write it
}

void IC_8255A::write_c(bool upper, uint8_t data){
  // disable write and read port
  digitalWrite(iocontrol[3],HIGH);
  digitalWrite(iocontrol[2],HIGH);

  // choose port c
  digitalWrite(iocontrol[0],LOW);
  digitalWrite(iocontrol[1],HIGH);

  int8_t check = upper == true ? 4 : 0;

  // any data that exceeds bit 3 will be discard
  for(uint8_t i = 0; i < 4; i++){
    digitalWrite(iodata[i+check],bitRead(data,i));
  }

  digitalWrite(iocontrol[2],LOW);
}

void IC_8255A::write_pin(char port, uint8_t pin, bool data){
  // disable write and read port
  digitalWrite(iocontrol[3],HIGH);
  digitalWrite(iocontrol[2],HIGH);

  if(pin > 7)
    return;


  if(port == 'A' || port == 'a'){
    digitalWrite(iocontrol[0],LOW);
    digitalWrite(iocontrol[1],LOW);
  } else if(port == 'B' || port == 'b') {
    digitalWrite(iocontrol[0],HIGH);
    digitalWrite(iocontrol[1],LOW);
  } else if(port == 'C' || port == 'c'){
    digitalWrite(iocontrol[0],LOW);
    digitalWrite(iocontrol[1],HIGH);
  } else {
    return; // port d.. doesn't exist
  }
  digitalWrite(iodata[pin],data);
  digitalWrite(iocontrol[2],LOW);
}

// read functions
int IC_8255A::read(char port){
  // assume that coder already used the pin_config function
  // i mean they should

  // disable write and read port
  digitalWrite(iocontrol[3],HIGH);
  digitalWrite(iocontrol[2],HIGH); 

  if(port == 'A' || port == 'a'){
    digitalWrite(iocontrol[0],LOW);
    digitalWrite(iocontrol[1],LOW);
  } else if(port == 'B' || port == 'b') {
    digitalWrite(iocontrol[0],HIGH);
    digitalWrite(iocontrol[1],LOW);
  } else if(port == 'C' || port == 'c'){
    digitalWrite(iocontrol[0],LOW);
    digitalWrite(iocontrol[1],HIGH);
  } else {
    return -1; // port d.. doesn't exist
  }

  digitalWrite(iocontrol[3],LOW); //read it
  // return type should be an int. since i don't know what coders are going to use the value for
  // create a var of 0x00 -> if high = '1', if low = '0', or result with var then left shift it
  uint8_t ret = 0x00;

  for(int i = 7; i > -1; i--){ //feed the data
    ret <<= 1; //left shift by one
    ret |= digitalRead(iodata[i]) == HIGH ? 1 : 0; // or assign with digital read ternary test
  }

  return int(ret);
}

int IC_8255A::read_c(bool upper){
  // disable write and read port
  digitalWrite(iocontrol[3],HIGH);
  digitalWrite(iocontrol[2],HIGH); 

  digitalWrite(iocontrol[0],LOW);
  digitalWrite(iocontrol[1],HIGH);

  digitalWrite(iocontrol[3],LOW); //read it
  // return type should be an int. since i don't know what coders are going to use the value for
  // create a var of 0x00 -> if high = '1', if low = '0', or result with var then left shift it
  uint8_t ret = 0x00;
  int8_t check = upper == true ? 4 : 0;
  // if upper 3 + 4 = 7; still reading msb to lsb

  for(int i = 3; i > -1; i--){ //feed the data
    ret <<= 1; //left shift by one
    ret |= digitalRead(iodata[i+check]) == HIGH ? 1 : 0; // or assign with digital read ternary test
  }

  return int(ret);
}

int IC_8255A::read_pin(char port, uint8_t pin){
  // disable write and read port
  digitalWrite(iocontrol[3],HIGH);
  digitalWrite(iocontrol[2],HIGH);

  if(pin > 7) //exceeded iodata index
    return -1;

  if(port == 'A' || port == 'a'){
    digitalWrite(iocontrol[0],LOW);
    digitalWrite(iocontrol[1],LOW);
  } else if(port == 'B' || port == 'b') {
    digitalWrite(iocontrol[0],HIGH);
    digitalWrite(iocontrol[1],LOW);
  } else if(port == 'C' || port == 'c'){
    digitalWrite(iocontrol[0],LOW);
    digitalWrite(iocontrol[1],HIGH);
  } else {
    return -1; // port d.. doesn't exist
  }

  digitalWrite(iocontrol[3],LOW); //read it

  return bool(digitalRead(iodata[pin]));
}

// mode b specific