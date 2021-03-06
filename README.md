# IC_8255A
An Arduino library for Intel 8255A, Programmable Peripheral Interface.

For technical details, please refer to the [datasheet.](https://pdf1.alldatasheet.com/datasheet-pdf/view/66100/INTEL/8255A.html)

## About
IC_8255A is an Arduino library for controlling Intel 8255A, Programmable Peripheal Interface (or Port Expander).

This library supports all modes offered by 8255A, in both reading and writing functionality.<br>

## How To Use The Library

### Creating an object
To save you from headache, the object requires two int array arguments to set everything up correctly.

First array contains the pin number of Arduino that are connected to D0-D7 pins of 8255A in order. The array size must equal to eight.

Second array contains the pin number of Arduino that are connected to a0, a1, ~wr and ~cs in order. The array size must equal to four.

Example:
```C
// d0 = pin 3, d1 = pin 4, d2 = pin 5, ...
int dataio[8] = {3,4,5,6,7,8,9,10};
// a0 = pin 11, a1 = pin 12, ~wr = pin 13, ~rd = pin 14
int reg[4] = {11,12,13,14};
```

After that, create an object of the library by parsing both arrays to its argument.
```C
// data pins, then control pins
IC_8255A ppi(dataio,reg);
```
**^If this library doesn't work, make sure ~cs pin is connected to the ground or logic 0.**

### Using the methods
This library has 2 important main methods: **pin_config** and **mode_config.**

'pin_config' is a method for configuring your Arduino pins mode (INPUT or OUTPUT).

'mode_config' is a method for configuring your 8255A pins data direction.

It's recommended to use pin_config method before mode_config method when configuring 8255A ports to make sure your Arduino pins doesn't have any errors when reading or writing.

An example sketch of configuring 8255A to set a port to work in write mode:
```C
void setup(){
	ppi.pin_config(0x00); // 0 = write, 1 = read
	ppi.mode_config(0x80); // setting all 8255a ports to write
	ppi.write('A',0xFF); // writing '0xFF' to 8255A
	ppi.write('B',0x0F); // writing '0x0F' to 8255A
	ppi.write('C',0xF0); // writing '0xF0' to 8255A
	...
}
```

An example sketch of configuring 8255A to set a port to work in read mode:
```C
void setup(){
	ppi.pin_config(0x00); // we need to configure the 8255a pin first
	ppi.mode_config(0x82); // setting port b to read only
	ppi.pin_config(0xFF); // configuring all Arduino port to read
	int portb_val = ppi.read('B'); // read value from port B then store it for further usage.
	...
}
```

# Methods
```C
IC_8255A(int data[], int spr[]);
```
Initiate the 8255A library.

```C
void mode_config(uint8_t word);
```
Configure the 8255A mode.

### Mode 0 Configuration Table
|Port A|Port B|Port C (Upper)|Port C (Lower)|Value|
|------|------|--------------|--------------|-----|
|OUTPUT|OUTPUT|        OUTPUT|        OUTPUT| 0x80|
|OUTPUT|OUTPUT|        OUTPUT|         INPUT| 0x81|
|OUTPUT| INPUT|        OUTPUT|        OUTPUT| 0x82|
|OUTPUT| INPUT|        OUTPUT|         INPUT| 0x83|
|OUTPUT|OUTPUT|         INPUT|        OUTPUT| 0x88|
|OUTPUT|OUTPUT|         INPUT|         INPUT| 0x89|
|OUTPUT| INPUT|         INPUT|        OUTPUT| 0x8A|
|OUTPUT| INPUT|         INPUT|         INPUT| 0x8B|
| INPUT|OUTPUT|        OUTPUT|        OUTPUT| 0x90|
| INPUT|OUTPUT|        OUTPUT|         INPUT| 0x91|
| INPUT| INPUT|        OUTPUT|        OUTPUT| 0x92|
| INPUT| INPUT|        OUTPUT|         INPUT| 0x93|
| INPUT|OUTPUT|         INPUT|        OUTPUT| 0x98|
| INPUT|OUTPUT|         INPUT|         INPUT| 0x99|
| INPUT| INPUT|         INPUT|        OUTPUT| 0x9A|
| INPUT| INPUT|         INPUT|         INPUT| 0x9B|

For the other modes, please refer to the datasheet.

```C
void pin_config(uint8_t data);
```
Configure the Arduino pins that are currently being used as data pins.<br>
This method works similar to how you would configure data directions for a microprocessor IC's I/O ports where logic '1' is read, and logic '0' is write.

For example:
```C
ppi.pin_config(0b01001001); // Set pin D0, D3 and D6 to read, else to write (from lsb to msb)
```

```C
void write(char port, uint8_t data);
```
Write a byte data to a port.

**^Choosing non-existing ports won't write the data.**

### 'port' Parameter Table
|'port' value  |Port|
|--------------|----|
| 'a' or 'A'   |  A |
| 'b' or 'B'   |  B |
| 'c' or 'C'   |  C |

**^This table applies to every  methods below too.**

```C
void write_c(bool upper, uint8_t data);
```
Write a nibble to upper (PC4 to PC7) or lower (PC0 to PC3) pins of port C.

**^Argument value for 'data' parameter should be in range of 0 to 15.**

```C
void write_pin(char port, uint8_t pin, bool data);
```
Write a boolean to a specific 8255A pin. Starting from zero to seven (refer to data bus pin numbers of 8255A).

**^If 'pin' parameter value is greater than 7 or choosing non-existent ports, this method won't write the data.**

```C
int read(char port);
```
Read a byte data from a port.

**^Choosing non-existing ports will return as -1.**

```C
int read_c(bool upper);
```
Read a nibble either from (PC4 to PC7) or lower (PC0 to PC3) pins of port C.

```C
int read_pin(char port, uint8_t pin);
```
Read a boolean from a specific 8255A pin. Starting from zero to seven (refer to data bus pin numbers of 8255A).

**^If 'pin' parameter value is greater than 7 or choosing non-existent ports, this method will return as -1.**
