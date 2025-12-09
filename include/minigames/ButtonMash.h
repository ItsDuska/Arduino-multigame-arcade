#ifndef GAME1_BUTTON_MASH_H
#define GAME1_BUTTON_MASH_H

#include "GameInterface.h"
#include <initializer_list>

class ButtonMashGame : public Game {
private:
  enum GameState { COUNTDOWN, PLAYING, GAMEOVER };
  GameState state;
  uint8_t keyPresses;
  bool isDirty = true;
  bool initialRender;

  const uint16_t GAME_DURATION = 10000; // 10 seconds
  const uint8_t TEXT_SIZE = 4;

  // Countdown until game starts
  uint8_t countdown = 3;
  uint32_t currentMillis;
  uint32_t previousMillis;
public:
  ButtonMashGame();
  void init(Arduino_GFX &gfx) override;
  void update(Keyboard &keyboard, Joystick &Joystick) override;
  void render(Arduino_GFX &gfx) override;
  void cleanup() override;
  const char *getName() override;
};

#endif
