#pragma once
#include "MockDisplay.h"
#include <stdint.h>

class Arduino_DataBus {
public:
  Arduino_DataBus() {}
  virtual ~Arduino_DataBus() {}
};

class Arduino_SWPAR8 : public Arduino_DataBus {
public:
  // Konstruktori ottaa vastaan ison läjän pinnejä
  Arduino_SWPAR8(int8_t dc, int8_t cs, int8_t wr, int8_t rd, int8_t d0,
                 int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5,
                 int8_t d6, int8_t d7) {}
};

// Perii Arduino_GFX:n, jotta piirtokomennot toimivat
class Arduino_R61529 : public Arduino_GFX {
public:
  // Konstruktori vastaa GameManagerin kutsua: (bus, rst, rotation, ips)
  Arduino_R61529(Arduino_DataBus *bus, int8_t rst, uint8_t r, bool ips)
      : Arduino_GFX(320, 240) // Asetetaan resoluutio (leveys, korkeus)
  {}
};
