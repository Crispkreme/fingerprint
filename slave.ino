#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "HD44780_LCD_PCF8574.h"

#define FILE_NAME "test.txt"
#define SD_CS_PIN 4
#define LCD_COLS 16
#define LCD_ROWS 2
#define LCD_ADDRESS 0x27
#define DISPLAY_DELAY_1 500 // Adjust as needed

File myFile;
HD44780LCD myLCD(LCD_COLS, LCD_ROWS, LCD_ADDRESS, &Wire);

void writeToFile(const char* message) {
  myFile = SD.open(FILE_NAME, FILE_WRITE);
  if (myFile) {
    myFile.println(message);
    myFile.close();
  } else {
    Serial.println("Error opening file");
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS_PIN)) {
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
    char message[128]; // Adjust size as per message length
    Serial.readBytesUntil('\n', message, sizeof(message));

    if (strncmp(message, "[MASTER]", 8) == 0) {
      Serial.println(message);

      // Extract user id and purpose
      char userId[64]; // Adjust size as per expected length
      char purpose[64]; // Adjust size as per expected length

      char* idStart = strstr(message, "user id: ") + 9;
      char* idEnd = strchr(idStart, '\n');
      if (idStart != nullptr && idEnd != nullptr) {
        strncpy(userId, idStart, idEnd - idStart);
        userId[idEnd - idStart] = '\0'; // Null-terminate the string
      }

      char* purposeStart = strstr(message, "purpose: ") + 9;
      char* purposeEnd = strchr(purposeStart, '\n');
      if (purposeStart != nullptr && purposeEnd != nullptr) {
        strncpy(purpose, purposeStart, purposeEnd - purposeStart);
        purpose[purposeEnd - purposeStart] = '\0'; // Null-terminate the string
      }

      myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);
      myLCD.PCF8574_LCDSendString("User ID: ");
      myLCD.PCF8574_LCDSendString(userId);
      myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo , 0);
      myLCD.PCF8574_LCDSendString("Purpose: ");
      myLCD.PCF8574_LCDSendString(purpose);
      myLCD.PCF8574_LCDSendChar('!');

      delay(DISPLAY_DELAY_1);

      writeToFile(message);
    }
  }
}
