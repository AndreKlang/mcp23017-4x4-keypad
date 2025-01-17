#ifndef ELECTRIC_LOAD_KEYPAD_H
#define ELECTRIC_LOAD_KEYPAD_H

#include "MCP23017.h"

namespace klang {
  enum IO_Bank {
    BankA,
    BankB,
  };

  enum Key {
    Key_1,
    Key_2,
    Key_3,
    Key_A,
    Key_4,
    Key_5,
    Key_6,
    Key_B,
    Key_7,
    Key_8,
    Key_9,
    Key_C,
    Key_Star,
    Key_0,
    Key_Pound,
    Key_D,
  };

  const char KeyMap[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D',
  };

  class Keypad {
    private:
      ::MCP23017* io;
      uint8_t port;
      uint16_t currentKeys = 0;
      std::function<void(Key)> onPressCallback = nullptr;
      std::function<void(Key)> onReleaseCallback = nullptr;

      static uint16_t mapKey(int col, int row);
      void updateKeys(uint16_t keys);
      void reset();
    public:
      Keypad(::MCP23017* _io, IO_Bank _bank);
      void begin();
      void interrupt();
      void onPress(const std::function<void(Key)> &press) {
        onPressCallback = press;
      };
      void onRelease(const std::function<void(Key)> &release) {
        onReleaseCallback = release;
      };
      static char mapFrom(Key key);
  };

}

#endif
