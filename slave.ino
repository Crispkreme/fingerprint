#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

File myFile;
LiquidCrystal_I2C lcd(0x3F, 16, 2);

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

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello World");

}

void loop() {
  if (Serial.available()) {

    String message = Serial.readStringUntil('\n');

    if (message.startsWith("[MASTER]")) {

      Serial.println(message);

      writeToFile(message);
    }
  }
}
