char mystr1[10];
char mystr2[10];

void setup() {
  Serial.begin(9600);
}

void loop() {

  Serial.readBytes(mystr1, 1);
  Serial.println(mystr1); 

  Serial.readBytes(mystr2, 6);
  Serial.println(mystr2); 

  delay(1000);
}
