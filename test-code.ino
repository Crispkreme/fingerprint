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

#define records 10 // 10 for 10 user

int user1, user2, user3, user4, user5, user6, user7, user8, user9, user10;

DateTime now;

// Define variables
const int buttonRegisterBack = A0; 
const int buttonForward = A1; 
const int buttonReverse = A2; 
const int buttonDeleteOK = A3; 

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

  pinMode(buzzer, OUTPUT);
  pinMode(indFinger, OUTPUT);

  if (rtc.lostPower()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(2018, 6, 7, 11, 0, 0));
  }
}

void loop() {
  now = rtc.now();

  // Read the state of the button
  int buttonStateRegisterBack = digitalRead(buttonRegisterBack);
  int buttonStateDeleteOK = digitalRead(buttonDeleteOK);

  if (digitalRead(buttonRegisterBack) == LOW) {

    donwloadExistingData();
    delay(1000);
    enrollData();

    Serial.println("buttonStateRegisterBack is pressed");
  }

  if (digitalRead(buttonDeleteOK) == LOW) {

    resettingExistingData();
    delay(1000);

    Serial.println("buttonStateDeleteOK is pressed");
  }

  for (int i = 1000; i < 1000 + records; i++) {
    if (EEPROM.read(i) == 0xff) EEPROM.write(i, 0);
  }
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

  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    return -1;
  }

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

    } else if (digitalRead(buttonStateDeleteOK) == LOW) {

      id = count;
      getFingerprintEnroll();
      for (int i = 0; i < records; i++) {
        if (EEPROM.read(i) != 0xff) {
          EEPROM.write(i, id);
          break;
        }
      }
      return;
    } else if (digitalRead(buttonStateRegisterBack) == LOW) {
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

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();

    switch (p) {
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_NOFINGER:
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        break;
      case FINGERPRINT_IMAGEFAIL:
        break;
      default:
        break;
    }
  }

  p = finger.image2Tz(1);

  switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_IMAGEMESS:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      return p;
    case FINGERPRINT_FEATUREFAIL:
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      return p;
    default:
      return p;
  }

  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = finger.image2Tz(2);

  switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_IMAGEMESS:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      return p;
    case FINGERPRINT_FEATUREFAIL:
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      return p;
    default:
      return p;
  }

  p = finger.createModel();
  if (p != FINGERPRINT_OK) {
    return p;
  }

  p = finger.storeModel(id);
  if (p != FINGERPRINT_OK) {
    return p;
  }
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) {
    return p;
  }
  return p;
}
