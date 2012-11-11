// Pinout
// 0, 1 => Serial RX, TX
//    3 => Bluetooth PIO2
//    5 => Bluetooth RESET
//    6 => Blue LED
static const int BT_PIO2 = 3;
static const int BT_RESET = 5;
static const int BLUE_LED = 6;

void BlinkLed(int Pin, unsigned int Times, unsigned int Period = 200) {
  for (int i = 0; i < Times; ++i) {
    digitalWrite(Pin, HIGH);
    delay(Period / 2);
    digitalWrite(Pin, LOW);
    delay(Period / 2);
  }
}

void SetupPinout() {
  pinMode(BT_PIO2, INPUT);
  pinMode(BT_RESET, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, LOW);
}

void SetupBluetooth() {
  Serial.begin(115200);
  digitalWrite(BT_RESET, LOW);
  delay(50);
  digitalWrite(BT_RESET, HIGH);
  delay(2000);
  Serial.flush();
}

void WaitBluetoothConnect() {
  while (digitalRead(BT_PIO2) == LOW) {
    delay(100);
  }
}

void setup() {
  SetupPinout();
  SetupBluetooth();
}

void loop() {
  WaitBluetoothConnect();
  BlinkLed(BLUE_LED, 3);
  Serial.println("Hello, Bluetooth!");
  
  // Receive commands
  while (true) {
    Serial.print("Enter command: ");
    String command = WaitBluetoothCommand();
    if (command != "") {
      boolean success = ProcessCommand(command);
      if (!success) {
        Serial.print("Error processing command: ");
        Serial.println(command);
      }
    }
  }
}

