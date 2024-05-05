#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "HD44780_LCD_PCF8574.h"

File myFile;
HD44780LCD myLCD(4, 16, 0x27, &Wire);

#define DISPLAY_DELAY_INIT 50

void writeToFile(String message) {
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("Initialization failed!");
    while (true);
  }

  myFile = SD.open("test.txt", FILE_WRITE);

  if (myFile) {
    myFile.println(String(message));
    myFile.close();
  } else {
    Serial.println("Error opening file");
  }
}

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  delay(DISPLAY_DELAY_INIT);
  myLCD.PCF8574_LCDInit(myLCD.LCDCursorTypeOn);
  myLCD.PCF8574_LCDClearScreen();
  myLCD.PCF8574_LCDBackLightSet(true);
  myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);
}

void loop() {

  String titleString = "SPELVA";

  myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);
  myLCD.PCF8574_LCDSendString(titleString.c_str());
  myLCD.PCF8574_LCDSendChar(' ');
  
  if (Serial.available()) {

    String message = Serial.readStringUntil('\n');

    if (message.startsWith("[MASTER]")) {

      Serial.println(message);

      writeToFile(message);
    }
  }
}
