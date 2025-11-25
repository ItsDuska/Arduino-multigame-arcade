#pragma once

#include "GameInterface.h"

class ReactionTimeGame : public Game
{
public:
  ReactionTimeGame();
  void init() override;
  void update(uint32_t deltaTime, Keyboard& keyboard, Joystick& joystick) override;
  void render(uint32_t deltaTime, Arduino_GFX& gfx) override;
  void cleanup() override;
  bool isComplete() override;
  const char* getName() override;

private:
  const uint8_t LED_PIN = 13;
  const uint16_t WAIT_TIME_MIN = 2000;
  const uint16_t WAIT_TIME_MAX = 5000;

  static constexpr char REACT_KEY = 'A';

  enum GamePhase {
    WAITING,
    LED_ON,
    COMPLETE
  };

  GamePhase phase;
  uint32_t phaseStartTime;
  uint32_t waitDuration;
  uint32_t reactionTime;
  bool gameComplete;
};

