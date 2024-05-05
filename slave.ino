#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "HD44780_LCD_PCF8574.h"

File myFile;
HD44780LCD myLCD( 2, 16, 0x27, &Wire);

void writeToFile(String message) {

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

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("Initialization failed!");
    while (true);
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

      int idPos = message.indexOf("user id: ");
      if (idPos != -1) {
        idPos += 9; // Move past "user id: "
        int idEnd = message.indexOf('\n', idPos);
        if (idEnd != -1) {
          userId = message.substring(idPos, idEnd);
        }
      }

      int purposePos = message.indexOf("purpose: ");
      if (purposePos != -1) {
        purposePos += 9; // Move past "purpose: "
        int purposeEnd = message.indexOf('\n', purposePos);
        if (purposeEnd != -1) {
          purpose = message.substring(purposePos, purposeEnd);
        }
      }

      // Display on LCD
      myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);
      myLCD.PCF8574_LCDSendString("User ID: " + userId);
      myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo , 0);
      myLCD.PCF8574_LCDSendString("Purpose: " + purpose);
      myLCD.PCF8574_LCDSendChar('!');

      delay(1000);

      // Write to file
      writeToFile(message.c_str());
    }
  }
}
