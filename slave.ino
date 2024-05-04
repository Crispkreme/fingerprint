#include <SPI.h>
#include <SD.h>

File myFile;

void writeToFile(String message) {
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("Initialization failed!");
    while (true);
  }

  myFile = SD.open("test.txt", FILE_WRITE);

  if (myFile) {
    myFile.println(String(message));
    myFile.println(" ");
    myFile.close();
    Serial.println("Data written to file.");
  } else {
    Serial.println("Error opening file");
  }
}

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

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
