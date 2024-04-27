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
  Serial.begin(9600);

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
  int result = getFingerprintIDez();

  // Read the state of the button
  int buttonStateRegisterBack = digitalRead(buttonRegisterBack);
  int buttonStateForward = digitalRead(buttonForward);
  int buttonStateReverse = digitalRead(buttonReverse);
  int buttonStateDeleteOK = digitalRead(buttonDeleteOK);

  Serial.println("buttonStateRegisterBack: " + buttonStateRegisterBack);
  Serial.println("buttonStateForward: " + buttonStateForward);
  Serial.println("buttonStateReverse: " + buttonStateReverse);
  Serial.println("buttonStateDeleteOK: " + buttonStateDeleteOK);

  // Check if the button is pressed
  if (buttonStateRegisterBack == HIGH){

    digitalWrite(ledPin, HIGH);   
    donwloadExistingData();
    Serial.println("button high");

  } else {
    digitalWrite(ledPin, LOW);   
    Serial.println("button LOW");
  }

  // Check if the button is pressed
  if (buttonStateForward == HIGH){
    digitalWrite(ledPin, HIGH);  
    Serial.println("button high"); 
  } else {
    digitalWrite(ledPin, LOW);   
    Serial.println("button LOW");
  }

  // Check if the button is pressed
  if (buttonStateReverse == HIGH){
    digitalWrite(ledPin, HIGH);  
    Serial.println("button high"); 
  } else {
    digitalWrite(ledPin, LOW);   
    Serial.println("button LOW");
  }

  // Check if the button is pressed
  if (buttonStateDeleteOK == HIGH){
    digitalWrite(ledPin, HIGH);  
    Serial.println("button high"); 

    resettingExistingData();
  } else {
    digitalWrite(ledPin, LOW);   
    Serial.println("button LOW");
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
