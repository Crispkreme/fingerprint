## INSTRUCTION
- open your arduino ide
- go to libraries
- find fingerprint by adafruit
- paste this code for registering the fingerprint image
- copy the register.ino into your arduino uno file
- then upload
- then open your serial monitor then type your i you want to stored your data 
example: Before enrolling any fingerprint, you must first enter its ID. You are allowed to choose the number 1 to 127. For example, in the image below, we have selected number 3.
- Place finger on sensor
- Place finger on the sensor again
note: Now you have to put your finger on the sensor again for the second image. When this step is completed correctly, the phrase Stored will appear.

## PIN CONFIGURATION
Command:  
```sh
ARDUINO TO AS608 Fingerprint Module

ARDUINO UNO ============> AS608 Fingerprint Module
5V          ============> VCCC
2           ============> TX
3           ============> RX
GND         ============> GND

ARDUINO TO DS1302-RTC Module
ARDUINO UNO ============> DS1302-RTC Module
5           ============> DS1302 CLK/SCLK
4           ============> DS1302 DAT/IO
2           ============> DS1302 RST/CE
5v          ============> VCC
GND         ============> GND
```