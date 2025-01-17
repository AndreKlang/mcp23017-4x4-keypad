#include <Arduino.h>
#include "MCP23017.h"
#include "Keypad.h"

#define PIN_I2C_SCL 25
#define PIN_I2C_SDA 33
#define PIN_IO_EXP_IRQ_B 35

MCP23017* io;
klang::Keypad* keypad;

volatile bool readKeypad = false;

void IRAM_ATTR isrKeypad() {
  readKeypad = true;
}

void setup() {
  Serial.begin(115200);

  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL, 100000);

  io = new MCP23017(0x20);
  io->begin(false);
  pinMode(PIN_IO_EXP_IRQ_B, INPUT_PULLUP);
  attachInterrupt(PIN_IO_EXP_IRQ_B, isrKeypad, CHANGE);

  keypad = new klang::Keypad(io, klang::BankB);
  keypad->begin();

  keypad->onPress([](klang::Key key){
    Serial.printf("Press: %c\n", klang::Keypad::mapFrom(key));
  });
  keypad->onRelease([](klang::Key key){
    Serial.printf("Release: %c\n", klang::Keypad::mapFrom(key));
  });
}

void loop() {
  if (readKeypad) {
    keypad->interrupt();
    readKeypad = false;
  }
}

