String WaitBluetoothCommand() {
  char buffer[100] = {0};
  int index = 0;
  while (index < 100) {
    if (Serial.available()) {
      byte val = Serial.read();
      Serial.write(val);
      if (val == '\n' || val == '\r') {
        buffer[index + 1] = '\0';
        String command(buffer);
        command.toUpperCase();
        return command;
      }
      buffer[index++] = val;
    }
  }
  return String("");
}

// Define our commands
enum CommandType {
  COMMAND_BLINK = 0,
  COMMANDS_COUNT
};

static const String COMMAND_TYPE_STRINGS[COMMANDS_COUNT] = {
  "BLINK"
};

void ParseCommand(const String& Command, String& Type, int& Value) {
  Type = Command;
  Value = 0;
  int commaPos = Command.indexOf(',');
  if (commaPos != -1) {
    Type = Command.substring(0, commaPos);
    if (commaPos + 1 < Command.length()) {
      Value = Command.substring(commaPos + 1).toInt();
    }
  }
}

// Blink the LED the given amount of times
// Command: BLINK,<period>
// period: toggle period, in ms, or 0 to disable
boolean ProcessBlinkCommand(const int& Value) {
  if (Value < 0) {
    Serial.println("Error processing BLINK command, period < 0");
    return false;
  }
  Serial.println("Processing BLINK command:");
  Serial.print("  period = ");
  Serial.println(Value);
  s_blinkTicks = Value;
  return true;
}

boolean ProcessCommand(const String& Command) {
  // Parse the command type
  String type;
  int value;
  ParseCommand(Command, type, value);
  for (int i = 0; i < COMMANDS_COUNT; ++i) {
    if (type.equals(COMMAND_TYPE_STRINGS[i])) {
      return ProcessCommand(i, value);
    }
  }
  return false;
}

boolean ProcessCommand(int Type, const int& Value) {
  // Call the specific command function
  switch (Type) {
    case COMMAND_BLINK:
      return ProcessBlinkCommand(Value);
    default:
      return false;
  }
}

