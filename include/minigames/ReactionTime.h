#pragma once

#include "GameInterface.h"

class ReactionTimeGame : public Game {
public:
  ReactionTimeGame();
  void init(Arduino_GFX &gfx) override;
  void update(uint32_t deltaTime, Keyboard &keyboard,
              Joystick &joystick) override;
  void render(uint32_t deltaTime, Arduino_GFX &gfx) override;
  void cleanup() override;
  const char *getName() override;

private:
  const uint16_t MAX_REACT_TIME = 50000;
  const uint8_t TEST_ROUNDS = 5;

  enum GamePhase { START, WAITING, REACTSCREEN, SPEED_DISPLAY, COMPLETE };

  GamePhase phase;
  uint32_t phaseStartTime;
  uint32_t reactionTime;
  uint32_t testCount;
  bool speedDisplayed;
};
