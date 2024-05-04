#include "Adafruit_Fingerprint.h" 
#include <EEPROM.h> 
#include <SoftwareSerial.h>
#include <Wire.h>
#include "RTClib.h"
RTC_DS3231 rtc;

uint8_t id;
SoftwareSerial fingerPrint(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);

#define indFinger 7
#define buzzer 5
#define match 5
#define records 10 // 10 for 10 user

int user1, user2, user3, user4, user5, user6, user7, user8, user9, user10;

DateTime now;

// Define variables
const int buttonRegisterBack = A0; 
const int buttonForward = A1; 
const int buttonReverse = A2; 
const int buttonDeleteOK = A3; 

const int buttonRoom1 = 7; 
const int buttonRoom2 = 8; 
const int buttonRoom3 = 9; 
const int buttonRoom4 = 10; 
const int buttonRoom5 = 11; 

const int buttonPins[] = {buttonRoom1, buttonRoom2, buttonRoom3, buttonRoom4, buttonRoom5};
const String roomNames[] = {"Room 1", "Room 2", "Room 3", "Room 4", "Room 5"};

int ledPin = 13;  

void setup() {
  Serial.begin(9600);
  finger.begin(57600);

  rtc.begin();

  pinMode(ledPin, OUTPUT);    

  pinMode(buttonRegisterBack, INPUT_PULLUP);
  pinMode(buttonForward, INPUT_PULLUP);
  pinMode(buttonReverse, INPUT_PULLUP);
  pinMode(buttonDeleteOK, INPUT_PULLUP);

  pinMode(buttonRoom1, INPUT_PULLUP);
  pinMode(buttonRoom2, INPUT_PULLUP);
  pinMode(buttonRoom3, INPUT_PULLUP);
  pinMode(buttonRoom4, INPUT_PULLUP);
  pinMode(buttonRoom5, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);
  pinMode(indFinger, OUTPUT);
  
  Serial.println("Purpose Built Fingerprint Scanner");
  Serial.println("Initializig your module .....");
  Serial.println("Finding module");
  
  for(int i=1000;i<1000 + records;i++)
  {
    if(EEPROM.read(i) == 0xff) EEPROM.write(i,0);
  }

  if (finger.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
    delay(1000);
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(2018, 6, 7, 11, 0, 0));
  }

  user1=EEPROM.read(1000);
  user2=EEPROM.read(1001);
  user3=EEPROM.read(1002);
  user4=EEPROM.read(1003);
  user5=EEPROM.read(1004);

  digitalWrite(indFinger, HIGH);
}

void donwloadExistingData()
{
  buzzerSound();

  displayMessage("Downloding Data..");
  Serial.print("S.No. ");

  for (int i = 0; i < records; i++) {
    
    buzzerSound();

    Serial.print(" User ID");
    Serial.print(i + 1);
    Serial.print(" ");
  }

  Serial.println();

  int eepIndex = 0;
  for (int i = 0; i < 30; i++) {
    if (i + 1 < 10) {
      Serial.print('0');
      Serial.print(i + 1);
      Serial.print(" ");

      eepIndex = (i * 7);
      download(eepIndex);

      eepIndex = (i * 7) + 210;
      download(eepIndex);

      eepIndex = (i * 7) + 420;
      download(eepIndex);

      eepIndex = (i * 7) + 630;
      download(eepIndex);

      eepIndex = (i * 7) + 840;
      download(eepIndex);

      eepIndex = (i * 7) + 1050;
      download(eepIndex);

      eepIndex = (i * 7) + 1260;
      download(eepIndex);

      eepIndex = (i * 7) + 1470;
      download(eepIndex);

      eepIndex = (i * 7) + 1680;
      download(eepIndex);

      Serial.println();
    }
  }

  delay(1000);
}

void resettingExistingData() 
{
  displayMessage("Reseting.....");

  for (int i = 1000; i < 1005; i++) EEPROM.write(i, 0);
  for (int i = 0; i < 841; i++) EEPROM.write(i, 0xff);

  delay(1000);
}

void buzzerSound()
{
  digitalWrite(buzzer, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
}

void displayMessage(String message)
{
  Serial.println("Please wait");
  Serial.println(message);
  Serial.println();
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
 
  if (p != FINGERPRINT_OK) return -1;
 
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;
 
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    Serial.println("Finger Not Found");
    Serial.println("Try Later");
    delay(1000);
    return -1;
  }
  
  // found a match!
  Serial.print("Found ID #");
  Serial.println(finger.fingerID);
  return finger.fingerID;
}

void download(int eepIndex) {
  if (EEPROM.read(eepIndex) != 0xff) {
    if (EEPROM.read(eepIndex) < 10)
      Serial.print('0');
      Serial.print(EEPROM.read(eepIndex++));
      Serial.print(':');
    if (EEPROM.read(eepIndex) < 10)
      Serial.print('0');
      Serial.print(EEPROM.read(eepIndex++));
      Serial.print(':');
    if (EEPROM.read(eepIndex) < 10)
      Serial.print('0');
      Serial.print(EEPROM.read(eepIndex++));
      Serial.print(" D->");
    if (EEPROM.read(eepIndex) < 10)
      Serial.print('0');
      Serial.print(EEPROM.read(eepIndex++));
      Serial.print('/');
    if (EEPROM.read(eepIndex) < 10)
      Serial.print('0');
      Serial.print(EEPROM.read(eepIndex++));
      Serial.print('/');
      Serial.print(EEPROM.read(eepIndex++) << 8 | EEPROM.read(eepIndex++));
  } else {
    Serial.print("---------------------------");
  }
  Serial.print(" ");
}

void enrollData() {

  int count = 1;

  int buttonStateRegisterBack = digitalRead(buttonRegisterBack);
  int buttonStateForward = digitalRead(buttonForward);
  int buttonStateReverse = digitalRead(buttonReverse);
  int buttonStateDeleteOK = digitalRead(buttonDeleteOK);

  while (1) {
    if (digitalRead(buttonForward) == LOW) {

      count++;
      Serial.println('User Id: ' + String(count));
      if (count > records) count = 1;
      delay(500);

    } else if (digitalRead(buttonReverse) == LOW) {

      count--;
      Serial.println('User Id: ' + String(count));
      if (count < 1) count = records;
      delay(500);

    } else if (digitalRead(buttonDeleteOK) == LOW) {

      id = count;
      getFingerprintEnroll();
      for (int i = 0; i < records; i++) {
        if (EEPROM.read(i) != 0xff) {
          EEPROM.write(i, id);
          break;
        }
      }
      return;
    } else if (digitalRead(buttonRegisterBack) == LOW) {
      return;
    }
  }
}

void attendance(int id) {
  int user = 0, eepLoc = 0;

  switch (id) {
    case 1:
      eepLoc = 0;
      user = user1++;
      break;
    case 2:
      eepLoc = 210;
      user = user2++;
      break;
    case 3:
      eepLoc = 420;
      user = user3++;
      break;
    case 4:
      eepLoc = 630;
      user = user4++;
      break;
    case 5:
      eepLoc = 0;
      user = user5++;
      break;
    case 6:
      eepLoc = 840;
      user = user5++;
      break;
    case 7:
      eepLoc = 1050;
      user = user7++;
      break;
    case 8:
      eepLoc = 1260;
      user = user8++;
      break;
    case 9:
      eepLoc = 1470;
      user = user9++;
      break;
    case 10:
      eepLoc = 1680;
      user = user8++;
      break;
    default:
      return;
  }

  int eepIndex = (user * 7) + eepLoc;

  EEPROM.write(eepIndex++, now.hour());
  EEPROM.write(eepIndex++, now.minute());
  EEPROM.write(eepIndex++, now.second());
  EEPROM.write(eepIndex++, now.day());
  EEPROM.write(eepIndex++, now.month());
  EEPROM.write(eepIndex++, now.year() >> 8 );
  EEPROM.write(eepIndex++, now.year());

  EEPROM.write(1000, user1);
  EEPROM.write(1001, user2);
  EEPROM.write(1002, user3);
  EEPROM.write(1003, user4);
}

uint8_t getFingerprintEnroll() {
  int p = -1;

  Serial.println("User id: " + id);
  delay(1000);

  Serial.println("Place Finger");
  delay(3000);

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();

    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println("No Finger");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(3000);

  p = 0;

  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  Serial.println("User id: " + id);
  Serial.println("Place same finger again");
  p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();

    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println("No Finger");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  p = finger.image2Tz(2);

  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Creating model for # " + id); 

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {

    Serial.println("Prints matched!");

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {

    Serial.println("Communication error");
    return p;

  } else if (p == FINGERPRINT_ENROLLMISMATCH) {

    Serial.println("Fingerprints did not match");
    return p;

  } else {

    Serial.println("Unknown error");
    return p;
  }

  Serial.println("User id: " + id);
  delay(1000);

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  Serial.println("Please wait");

  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
    Serial.print("Successfully");
    delay(1000);
  } else {
    Serial.println("Something Wrong");
    Serial.println("Try Again Later");
    delay(2000);
    return p;
  }
}

void loop() {
  now = rtc.now();
  int result = getFingerprintIDez();
  
  
  if (digitalRead(buttonRegisterBack) == LOW) {

    donwloadExistingData();
    delay(1000);
    enrollData();
  }

  if (digitalRead(buttonDeleteOK) == LOW) {

    resettingExistingData();
    delay(1000);
  }

  for (int i = 0; i < sizeof(buttonPins) / sizeof(buttonPins[0]); i++) {
    if (digitalRead(buttonPins[i]) == LOW) {

      if (result > 0) {

        digitalWrite(indFinger, LOW);
        buzzerSound();
        attendance(result);

        Serial.println("[MASTER] user id: " + String(result));
        Serial.println("[MASTER] purpose: " + roomNames[i]);
        Serial.println("[MASTER] date: April 28, 2024");

        digitalWrite(indFinger, HIGH);
        return;
      }
    }
  }

  for (int i = 1000; i < 1000 + records; i++) {
    if (EEPROM.read(i) == 0xff) EEPROM.write(i, 0);
  }
}

