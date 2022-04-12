#ifndef ft_8255A
#define ft_8255A

#include "Arduino.h"

// TODO: make reading or writing the port possible [x]
// TODO: make port c upper and lower input or output possible [x]
// TODO: implement mode b to port c []
// TODO: implement mode c to port c and turn port a into bi-directional beast []
// TODO: bsr mode for port c []
// TODO: read or write a single port because it's essential [x]

// pin config literals
#define WRITE 0x00
#define READ 0xFF
#define UPPER_C_READ 0xF0
#define LOWER_C_READ 0x0F


class IC_8255A{
  public: 
    // data = i/o pin, spr = i/o control
    IC_8255A(int data[], int spr[]);
    void mode_config(uint8_t word); // 8255a mode config
    void pin_config(uint8_t data); // arduino pin config
    // mode a
    void write(char port, uint8_t data);
    void write_c(bool upper, uint8_t data);
    void write_pin(char port, uint8_t pin, bool data);

    int read(char port);
    int read_c(bool upper);
    int read_pin(char port, uint8_t pin);
    // mode b port c is funky, port a and b is still functioning as mode a
    // mode c port c is funky, port a is bi-directional, port b is unused
  private:
    int iodata[8]; //pin info
    int iocontrol[4]; //pin info
};

#endif