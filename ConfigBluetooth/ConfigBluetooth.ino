// Pinout
// 0, 1 => Serial RX, TX
//    3 => Bluetooth PIO2
//    5 => Bluetooth RESET
//    6 => Blue LED
static const int BT_PIO2 = 3;
static const int BT_RESET = 5;
static const int BLUE_LED = 6;

void BlinkLed(int Pin, unsigned int Times, unsigned int Delay = 200) {
  for (int i = 0; i < Times; ++i) {
    digitalWrite(Pin, HIGH);
    delay(Delay);
    digitalWrite(Pin, LOW);
    delay(Delay);
  }
}

void SetupPinout() {
  pinMode(BT_PIO2, INPUT);
  pinMode(BT_RESET, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
}

void SetupBluetooth() {
  Serial.begin(115200);
  digitalWrite(BT_RESET, LOW);
  delay(50);
  digitalWrite(BT_RESET, HIGH);
  delay(2000);
  Serial.flush();
}

void setup() {
  SetupPinout();
  SetupBluetooth();
}

void ConfigBluetooth() {
  char output[100];
  memset(output, '\0', sizeof(output));

  // Send "$$$" to enter config mode
  // Send "S-,<name>\n" to configure friendly name
  // Send "SP,<pass>\n" to configure passcode
  // Send "---\n" to end session
  // NOTE: settings don't take effect until reboot
  Serial.write("$$$");
  delay(100);
  Serial.println("S-,Arduino");
  delay(100);
  Serial.println("SP,0000");
  delay(100);
  Serial.println("---");
  delay(1000);
  Serial.readBytes(output, 100);
  
  // Output what we received
  Serial.write("Read: ");
  Serial.println(output);  
}

void loop() {
  ConfigBluetooth();
  BlinkLed(BLUE_LED, 3);
  while (true) {
    delay(1000);
  }
}
