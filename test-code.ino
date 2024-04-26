#include "Adafruit_Fingerprint.h" 
#include <EEPROM.h> 
#include <SoftwareSerial.h>
#include <Wire.h>
#include "RTClib.h"
RTC_DS3231 rtc;

SoftwareSerial fingerPrint(2, 3); // for tx/rx communication between arduino & r305 fingerprint sensor

uint8_t id;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);

#define register_back 14
#define delete_ok 15
#define forward 16
#define reverse 17
#define match 5
#define indFinger 7
#define buzzer 5
#define records 10 

int user1, user2, user3, user4, user5, user6, user7, user8, user9, user10;

DateTime now;

// buttons functionality
const int button_register_back = A0;

void setup() {

  // delay(1000);
  Serial.begin(9600);
  pinMode(button_register_back, INPUT_PULLUP);

  // pinMode(register_back, INPUT_PULLUP);
  // pinMode(forward, INPUT_PULLUP);
  // pinMode(reverse, INPUT_PULLUP);
  // pinMode(delete_ok, INPUT_PULLUP);
  // pinMode(match, INPUT_PULLUP);
  // pinMode(buzzer, OUTPUT);
  // pinMode(indFinger, OUTPUT);
  // digitalWrite(buzzer, LOW);

  // if (digitalRead(register_back) == 0) {
  //   digitalWrite(buzzer, HIGH);
  //   delay(500);
  //   digitalWrite(buzzer, LOW);

  //   Serial.println("Please wait");
  //   Serial.println("Downloding Data..");
  //   Serial.println();

  //   Serial.print("S.No. ");

  //   for (int i = 0; i < records; i++) {
  //     digitalWrite(buzzer, HIGH);
  //     delay(500);
  //     digitalWrite(buzzer, LOW);
  //     Serial.print(" User ID");
  //     Serial.print(i + 1);
  //     Serial.print(" ");
  //   }

  //   Serial.println();

  //   int eepIndex = 0;
  //   for (int i = 0; i < 30; i++) {
  //     if (i + 1 < 10)
  //       Serial.print('0');
  //     Serial.print(i + 1);
  //     Serial.print(" ");
  //     eepIndex = (i * 7);
  //     download(eepIndex);
  //     eepIndex = (i * 7) + 210;
  //     download(eepIndex);
  //     eepIndex = (i * 7) + 420;
  //     download(eepIndex);
  //     eepIndex = (i * 7) + 630;
  //     download(eepIndex);
  //     eepIndex = (i * 7) + 840;
  //     download(eepIndex);
  //     eepIndex = (i * 7) + 1050;
  //     download(eepIndex);
  //     eepIndex = (i * 7) + 1260;
  //     download(eepIndex);
  //     eepIndex = (i * 7) + 1470;
  //     download(eepIndex);
  //     eepIndex = (i * 7) + 1680;
  //     download(eepIndex);
  //     Serial.println();
  //   }
  // }

  // if (digitalRead(delete_ok) == 0) {
  //   Serial.println("Please wait");
  //   Serial.println("Reseting.....");
  //   Serial.println();

  //   for (int i = 1000; i < 1005; i++)
  //     EEPROM.write(i, 0);

  //   for (int i = 0; i < 841; i++)
  //     EEPROM.write(i, 0xff);
  // }

  // digitalWrite(buzzer, HIGH);
  // delay(500);
  // digitalWrite(buzzer, LOW);
  // for (int i = 1000; i < 1000 + records; i++) {
  //   if (EEPROM.read(i) == 0xff)
  //     EEPROM.write(i, 0);
  // }

  // finger.begin(57600);
  // Serial.begin(9600);

  // if (! rtc.begin())
  //   Serial.println("Couldn't find RTC");

  // if (rtc.lostPower()) {
  //   Serial.println("RTC is NOT running!");
  //   rtc.adjust(DateTime(2018, 6, 7, 11, 0, 0));
  // }

}

void loop() {
  // now = rtc.now();

  // int result = getFingerprintIDez();
  // if (result > 0) {
  //   digitalWrite(indFinger, LOW);
  //   digitalWrite(buzzer, HIGH);
  //   delay(100);
  //   digitalWrite(buzzer, LOW);
  //   attendance(result);
  //   digitalWrite(indFinger, HIGH);
  //   return;
  // }
  // checkKeys();
  // delay(300);

  int buttonState = digitalRead(button_register_back); // Read the state of the button

  if (buttonState == LOW) {
    Serial.println("Button pressed"); 
  }
}

// void attendance(int id) {
//   int user = 0, eepLoc = 0;
//   if (id == 1) {
//     eepLoc = 0;
//     user = user1++;
//   } else if (id == 2) {
//     eepLoc = 210;
//     user = user2++;
//   } else if (id == 3) {
//     eepLoc = 420;
//     user = user3++;
//   } else if (id == 4) {
//     eepLoc = 630;
//     user = user4++;
//   } else if (id == 5) {
//     eepLoc = 0;
//     user = user5++;
//   } else if (id == 6) {
//     eepLoc = 840;
//     user = user5++;
//   } else if (id == 7) {
//     eepLoc = 1050;
//     user = user7++;
//   } else if (id == 8) {
//     eepLoc = 1260;
//     user = user8++;
//   } else if (id == 9) {
//     eepLoc = 1470;
//     user = user9++;
//   } else if (id == 10) {
//     eepLoc = 1680;
//     user = user8++;
//   } else {
//     return;
//   }

//   int eepIndex = (user * 7) + eepLoc;
//   EEPROM.write(eepIndex++, now.hour());
//   EEPROM.write(eepIndex++, now.minute());
//   EEPROM.write(eepIndex++, now.second());
//   EEPROM.write(eepIndex++, now.day());
//   EEPROM.write(eepIndex++, now.month());
//   EEPROM.write(eepIndex++, now.year() >> 8 );
//   EEPROM.write(eepIndex++, now.year());

//   EEPROM.write(1000, user1);
//   EEPROM.write(1001, user2);
//   EEPROM.write(1002, user3);
//   EEPROM.write(1003, user4);
// }

// void checkKeys() {
//   if (digitalRead(register_back) == 0) {
//     delay(1000);
//     while (digitalRead(register_back) == 0);
//     Enroll();
//   } else if (digitalRead(delete_ok) == 0) {
//     delay(1000);
//     delet();
//   }
// }

// void Enroll() {
//   int count = 1;

//   while (1) {
//     if (digitalRead(forward) == 0) {
//       count++;
//       if (count > records)
//         count = 1;
//       delay(500);
//     } else if (digitalRead(reverse) == 0) {
//       count--;
//       if (count < 1)
//         count = records;
//       delay(500);
//     } else if (digitalRead(delete_ok) == 0) {
//       id = count;
//       getFingerprintEnroll();
//       for (int i = 0; i < records; i++) {
//         if (EEPROM.read(i) != 0xff) {
//           EEPROM.write(i, id);
//           break;
//         }
//       }
//       return;
//     } else if (digitalRead(register_back) == 0) {
//       return;
//     }
//   }
// }

// void delet() {
//   int count = 1;

//   while (1) {
//     if (digitalRead(forward) == 0) {
//       count++;
//       if (count > records)
//         count = 1;
//       delay(500);
//     } else if (digitalRead(reverse) == 0) {
//       count--;
//       if (count < 1)
//         count = records;
//       delay(500);
//     } else if (digitalRead(delete_ok) == 0) {
//       id = count;
//       deleteFingerprint(id);
//       for (int i = 0; i < records; i++) {
//         if (EEPROM.read(i) == id) {
//           EEPROM.write(i, 0xff);
//           break;
//         }
//       }
//       return;
//     } else if (digitalRead(register_back) == 0) {
//       return;
//     }
//   }
// }

// uint8_t getFingerprintEnroll() {
//   int p = -1;

//   while (p != FINGERPRINT_OK) {
//     p = finger.getImage();
//     switch (p) {
//       case FINGERPRINT_OK:
//         break;
//       case FINGERPRINT_NOFINGER:
//         break;
//       case FINGERPRINT_PACKETRECIEVEERR:
//         break;
//       case FINGERPRINT_IMAGEFAIL:
//         break;
//       default:
//         break;
//     }
//   }

//   p = finger.image2Tz(1);

//   switch (p) {
//     case FINGERPRINT_OK:
//       break;
//     case FINGERPRINT_IMAGEMESS:
//       return p;
//     case FINGERPRINT_PACKETRECIEVEERR:
//       return p;
//     case FINGERPRINT_FEATUREFAIL:
//       return p;
//     case FINGERPRINT_INVALIDIMAGE:
//       return p;
//     default:
//       return p;
//   }

//   while (p != FINGERPRINT_NOFINGER) {
//     p = finger.getImage();
//   }

//   p = finger.image2Tz(2);

//   switch (p) {
//     case FINGERPRINT_OK:
//       break;
//     case FINGERPRINT_IMAGEMESS:
//       return p;
//     case FINGERPRINT_PACKETRECIEVEERR:
//       return p;
//     case FINGERPRINT_FEATUREFAIL:
//       return p;
//     case FINGERPRINT_INVALIDIMAGE:
//       return p;
//     default:
//       return p;
//   }

//   p = finger.createModel();
//   if (p != FINGERPRINT_OK) {
//     return p;
//   }

//   p = finger.storeModel(id);
//   if (p != FINGERPRINT_OK) {
//     return p;
//   }
// }

// int getFingerprintIDez() {
//   uint8_t p = finger.getImage();

//   if (p != FINGERPRINT_OK)
//     return -1;

//   p = finger.image2Tz();
//   if (p != FINGERPRINT_OK)
//     return -1;

//   p = finger.fingerFastSearch();
//   if (p != FINGERPRINT_OK) {
//     return -1;
//   }

//   return finger.fingerID;
// }

// uint8_t deleteFingerprint(uint8_t id) {
//   uint8_t p = -1;

//   p = finger.deleteModel(id);
//   if (p == FINGERPRINT_OK) {
//     return p;
//   }
//   return p;
// }

// void download(int eepIndex) {
//   if (EEPROM.read(eepIndex) != 0xff) {
//     if (EEPROM.read(eepIndex) < 10)
//       Serial.print('0');
//     Serial.print(EEPROM.read(eepIndex++));
//     Serial.print(':');
//     if (EEPROM.read(eepIndex) < 10)
//       Serial.print('0');
//     Serial.print(EEPROM.read(eepIndex++));
//     Serial.print(':');
//     if (EEPROM.read(eepIndex) < 10)
//       Serial.print('0');
//     Serial.print(EEPROM.read(eepIndex++));
//     Serial.print(" D->");
//     if (EEPROM.read(eepIndex) < 10)
//       Serial.print('0');
//     Serial.print(EEPROM.read(eepIndex++));
//     Serial.print('/');
//     if (EEPROM.read(eepIndex) < 10)
//       Serial.print('0');
//     Serial.print(EEPROM.read(eepIndex++));
//     Serial.print('/');
//     Serial.print(EEPROM.read(eepIndex++) << 8 | EEPROM.read(eepIndex++));
//   } else {
//     Serial.print("---------------------------");
//   }
//   Serial.print(" ");
// }
