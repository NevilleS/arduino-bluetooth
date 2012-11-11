// Pinout
// 0, 1 => Serial RX, TX
//    3 => Bluetooth PIO2
//    6 => White LED
//    5 => Bluetooth RESET
//    6 => Blue LED
static const int BT_PIO2 = 3;
static const int WHITE_LED = 4;
static const int BT_RESET = 5;
static const int BLUE_LED = 6;

// Timer variables
int s_ticks = 0;
int s_blinkTicks = 0;
boolean s_whiteLedState = 0;

// Synchronous blink
void BlinkLed(int Pin, unsigned int Times, unsigned int Period = 200) {
  for (int i = 0; i < Times; ++i) {
    digitalWrite(Pin, HIGH);
    delay(Period / 2);
    digitalWrite(Pin, LOW);
    delay(Period / 2);
  }
}

// Configure our pinout
void SetupPinout() {
  pinMode(BT_PIO2, INPUT);
  pinMode(BT_RESET, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(WHITE_LED, s_whiteLedState);
}

// Connect to the Bluetooth module and reset
void SetupBluetooth() {
  Serial.begin(115200);
  digitalWrite(BT_RESET, LOW);
  delay(50);
  digitalWrite(BT_RESET, HIGH);
  delay(2000);
  Serial.flush();
}

// Configure timer 2 to fire an interrupt every 1 ms
// From sample code at http://arduinomega.blogspot.ca/2011/05/timer2-and-overflow-interrupt-lets-get.html
void SetupTimer2() {
  TCCR2B = 0x00; // disable timer
  TCNT2  = 130;  // reset count to 130
  TIFR2  = 0x00; // clear interrupt flag
  TIMSK2 = 0x01; // enable interrupt
  TCCR2A = 0x00; // wave gen mode normal
  TCCR2B = 0x05; // prescaler set to 128
}

// Handle the timer 2 interrupt (every ~1 ms)
ISR(TIMER2_OVF_vect) {
  ++s_ticks;
  
  // Toggle the LED state if active
  if (s_blinkTicks && s_ticks % s_blinkTicks == 0) {
    s_whiteLedState = !s_whiteLedState;
    digitalWrite(WHITE_LED, s_whiteLedState);
  }
  
  // If disabled, make sure the LED is off
  if (s_blinkTicks == 0 && s_whiteLedState) {
    s_whiteLedState = 0;
    digitalWrite(WHITE_LED, s_whiteLedState);
  }
  
  // Rest the timer count to 130 and clear the interrupt flag
  TCNT2 = 130;
  TIFR2 = 0x00;
};

// Wait until the Bluetooth module is connected
void WaitBluetoothConnect() {
  while (digitalRead(BT_PIO2) == LOW) {
    delay(100);
  }
}

void setup() {
  SetupPinout();
  SetupTimer2();
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

