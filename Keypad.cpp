#include "Keypad.h"

namespace klang {
  Keypad::Keypad(::MCP23017 *_io, IO_Bank bank) {
    io = _io;
    port = bank == IO_Bank::BankA ? 0 : 1;
  }

  void Keypad::begin() {
    io->pinMode8(port, 0xF0); // 1-4 output, 5-8 input
    io->setPullup8(port, 0xF0);
    io->setPolarity8(port, 0xFF);
    io->enableInterrupt8(port, 0xF0, CHANGE);
    reset();
  }

  void Keypad::reset() {
    io->write8(port, 0xF0); // 1-4 LOW, 5-8 HIGH
    io->getInterruptCaptureRegister8(port); // clear the interrupt
  }

  void Keypad::interrupt() {
    uint16_t keys = 0;

    for (int col = 0; col < 4; col++) {
      io->write8(port, 0xFF & ~(1 << col)); // all HIGH except col
      uint8_t active = io->read8(port) >> 4;
      for (int row = 0; row < 4; row++) {
        if (active & 1 << row) {
          keys |= 1 << ((row * 4) + col);
        }
      }
    }

    reset();
    updateKeys(keys);
  }

  void Keypad::updateKeys(uint16_t keys) {
    if (currentKeys == keys) {
      return;
    }

    for (int i = 0; i < 16; ++i) {
      uint16_t key = 1 << i;
      bool before = (currentKeys & key) == key;
      bool after = (keys & key) == key;

      if (before && !after && onReleaseCallback != nullptr) {
        onReleaseCallback(Key(i));
      }

      if (!before && after && onPressCallback != nullptr) {
        onPressCallback(Key(i));
      }
    }

    currentKeys = keys;
  }

  char Keypad::mapFrom(Key key) {
    return klang::KeyMap[key];
  }
}