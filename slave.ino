#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "HD44780_LCD_PCF8574.h"

File myFile;
HD44780LCD myLCD( 2, 16, 0x27, &Wire);

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

  delay(50);
  myLCD.PCF8574_LCDInit(myLCD.LCDCursorTypeOn);
  myLCD.PCF8574_LCDClearScreen();
  myLCD.PCF8574_LCDBackLightSet(true);
}

void loop() {

  if (Serial.available()) {

    String message = Serial.readStringUntil('\n');

    if (message.startsWith("[MASTER]")) {

      Serial.println(message);

      // Extract user id and purpose
      String userId;
      String purpose;
      int idStart = message.indexOf("user id: ") + 9;
      int idEnd = message.indexOf('\n', idStart);
      if (idStart != -1 && idEnd != -1) {
        userId = message.substring(idStart, idEnd);
      }
      int purposeStart = message.indexOf("purpose: ") + 9;
      int purposeEnd = message.indexOf('\n', purposeStart);
      if (purposeStart != -1 && purposeEnd != -1) {
        purpose = message.substring(purposeStart, purposeEnd);
      }

      // Display user id and purpose on LCD
      myLCD.PCF8574_LCDGOTO(0, 0);
      myLCD.PCF8574_LCDSendString("User ID: ");
      myLCD.PCF8574_LCDGOTO(0, 9);
      myLCD.PCF8574_LCDSendString(userId.c_str());
      myLCD.PCF8574_LCDGOTO(1, 0);
      myLCD.PCF8574_LCDSendString("Purpose: ");
      myLCD.PCF8574_LCDGOTO(1, 9);
      myLCD.PCF8574_LCDSendString(purpose.c_str());

      delay(DISPLAY_DELAY_1);

      writeToFile(message);
    }
  }
}
