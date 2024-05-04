void setup() {
  Serial.begin(9600);
}

void loop() {

  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    if (message.startsWith("[MASTER]")) {
      // Print only messages intended for the master
      Serial.println(message);
    }
  }
}
