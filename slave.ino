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

      String userId;
      String purpose;

      int userIdPos = message.indexOf("user id: ");
      if (userIdPos != -1) {
        userIdPos += 9; 
        message.trim(); 
        userId = message.substring(userIdPos);

        int nonNumericIndex = -1;
        for (int i = 0; i < userId.length(); i++) {
          if (!isdigit(userId.charAt(i))) {
            nonNumericIndex = i;
            break;
          }
        }
        
        if (nonNumericIndex != -1) {
          userId = userId.substring(0, nonNumericIndex);
        }
      }
      
      int purposePos = message.indexOf("purpose: ");
      if (purposePos != -1) {
        purposePos += 9; 
        purpose = message.substring(purposePos);
        purpose.trim();
      }

      myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);
      myLCD.PCF8574_LCDSendString("User ID: ");
      myLCD.PCF8574_LCDSendString(userId.c_str());

      myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 0);
      myLCD.PCF8574_LCDSendString("Purpose: ");
      myLCD.PCF8574_LCDSendString(purpose.c_str());

      delay(1000);
    }
  }
}
