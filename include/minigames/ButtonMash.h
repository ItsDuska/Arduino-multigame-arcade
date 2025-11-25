#ifndef GAME1_BUTTON_MASH_H
#define GAME1_BUTTON_MASH_H

#include "GameInterface.h"

// Temp peli Testi peli. Ideana testata vaan että toimiiko kaikki.

class ButtonMashGame : public Game
{
private:
  const uint8_t BUTTON_PIN = 2;
  const uint16_t GAME_DURATION = 10000;  // 10 seconds

  uint16_t score;
  uint32_t startTime;
  bool gameComplete;
  bool lastButtonState;

public:
  ButtonMashGame();

  void init() override;

  void update(uint32_t deltaTime, Keyboard& keyboard, Joystick& Joystick) override;
  void render(uint32_t deltaTime, Arduino_GFX& gfx) override;

  void cleanup() override;

  bool isComplete() override;

  const char* getName() override;
};

#endif
