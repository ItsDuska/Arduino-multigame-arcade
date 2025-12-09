#pragma once

#include "GameInterface.h"

class ReactionTimeGame : public Game {
public:
  ReactionTimeGame();
  void init(Arduino_GFX &gfx) override;
  void update(Keyboard &keyboard, Joystick &joystick) override;
  void render(Arduino_GFX &gfx) override;
  void cleanup() override;
  const char *getName() override;

private:
  const uint16_t MAX_REACT_TIME = 50000;
  const uint8_t TEST_ROUNDS = 3;

  enum GamePhase { START, WAITING, REACTSCREEN, SPEED_DISPLAY, COMPLETE };

  GamePhase phase;
  uint32_t phaseStartTime;
  uint32_t reactionTime;
  uint32_t testCount;
  bool speedDisplayed;
  bool initialRender;
  bool isDirty;
};
