# MCP23017 4x4 Keypad Library

Published primarily for demonstration purposes.

* Entirely based on interrupts
* Sort of supports multiple simultaneous keys, but not on the same row.
  * Since I have no need for multiples at the moment, this is not something I'm looking into.
    
    (The issue is that no interrupt is fired on the second press)
* Connect columns to pins 1-4, and rows to pins 5-8 of either bank A or B of MCP23017
* Only tested on esp32, via platformio
* A main objective was to keep the I2C line as quiet as possible
* Depends on: https://github.com/RobTillaart/MCP23017_RT/pull/41