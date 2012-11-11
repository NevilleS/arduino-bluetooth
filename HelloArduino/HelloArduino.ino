void setup() {
  Serial.begin(9600);
  Serial.println("Hello, PC!");
}

void loop() {
  char buffer[50];
  memset(buffer, '\0', sizeof(buffer));
  byte bytesRead = 0;
  while (Serial.available() > 0) {
    bytesRead = Serial.readBytesUntil('\n', buffer, 50);
    buffer[bytesRead + 1] = '\0';
    Serial.print("Read: ");
    Serial.println(buffer); 
  }
}








