#include "Adafruit_Fingerprint.h" 
#include <EEPROM.h> 
#include <SoftwareSerial.h>
#include <Wire.h>
#include "RTClib.h"
RTC_DS3231 rtc;

uint8_t id;
SoftwareSerial fingerPrint(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);

#define match 5
#define indFinger 7
#define buzzer 5

#define records 10 // 10 for 10 user

int user1, user2, user3, user4, user5, user6, user7, user8, user9, user10;

DateTime now;

const int button_register_back = A0;
const int button_forward = A1;
const int button_reverse = A2;
const int button_delete_ok = A3;

void setup() {
  Serial.begin(9600);
  finger.begin(57600);
  Serial.begin(9600);

  pinMode(button_register_back, INPUT_PULLUP);
  pinMode(button_forward, INPUT_PULLUP);
  pinMode(button_reverse, INPUT_PULLUP);
  pinMode(button_delete_ok, INPUT_PULLUP);
  
  pinMode(match, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(indFinger, OUTPUT);

  int buttonRegisterBackState = digitalRead(button_register_back);
  int buttonForwardState = digitalRead(button_forward);
  int buttonReverseState = digitalRead(button_reverse);
  int buttonDeleteOkState = digitalRead(button_delete_ok);

  if (buttonRegisterBackState == HIGH) {
    
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
  }

  if (buttonDeleteOkState == HIGH) {

    displayMessage("Reseting.....");

    for (int i = 1000; i < 1005; i++) EEPROM.write(i, 0);
    for (int i = 0; i < 841; i++) EEPROM.write(i, 0xff);
  }

  buzzerSound();

  for (int i = 1000; i < 1000 + records; i++) {
    if (EEPROM.read(i) == 0xff) EEPROM.write(i, 0);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(2018, 6, 7, 11, 0, 0));
  }
}

void loop() {
  now = rtc.now();

  int buttonRegisterBackState = digitalRead(button_register_back);
  int buttonForwardState = digitalRead(button_forward);
  int buttonReverseState = digitalRead(button_reverse);
  int buttonDeleteOkState = digitalRead(button_delete_ok);

  int result = getFingerprintIDez();

  if (result > 0) {
    digitalWrite(indFinger, LOW);
    
    buzzerSound();

    attendance(result);
    digitalWrite(indFinger, HIGH);
    return;
  }

  checkKeys();
  delay(300);
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
void checkKeys() {

  int buttonRegisterBackState = digitalRead(button_register_back);
  int buttonForwardState = digitalRead(button_forward);
  int buttonReverseState = digitalRead(button_reverse);
  int buttonDeleteOkState = digitalRead(button_delete_ok);

  if (buttonRegisterBackState == HIGH) {
    actionData(1);
  } else if (buttonDeleteOkState == HIGH) {
    actionData(2);
  }
}
void actionData(int action) {

  int buttonRegisterBackState = digitalRead(button_register_back);
  int buttonForwardState = digitalRead(button_forward);
  int buttonReverseState = digitalRead(button_reverse);
  int buttonDeleteOkState = digitalRead(button_delete_ok);
  
  int count = 1;

  while (1) {
    if (buttonForwardState == HIGH) {
      count++;
      if (count > records) count = 1;
      delay(500);
    } else if (buttonReverseState == HIGH) {
      count--;
      if (count < 1) count = records;
      delay(500);
    } else if (buttonDeleteOkState == HIGH) {
      id = count;

      if(action == 1) {
        getFingerprintEnroll();

        for (int i = 0; i < records; i++) {
          if (EEPROM.read(i) != 0xff) {
            EEPROM.write(i, id);
            break;
          }
        }
        return;

      } else {
        deleteFingerprint(id);

        for (int i = 0; i < records; i++) {
          if (EEPROM.read(i) == id) {
            EEPROM.write(i, 0xff);
            break;
          }
        }
        return;
      }
      
      
    } else if (buttonRegisterBackState == HIGH) {
      return;
    }
  }
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
uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) {
    return p;
  }
  return p;
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
