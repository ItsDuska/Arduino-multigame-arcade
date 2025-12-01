#pragma once
#include "GameInterface.h"

class PlayerStatManager;
class EndScreen : public Game {
public:
  EndScreen(PlayerStatManager *statManager, uint16_t newScore);
  void init(Arduino_GFX &gfx) override;
  void update(uint32_t deltaTime, Keyboard &keyboard,
              Joystick &Joystick) override;
  void render(uint32_t deltaTime, Arduino_GFX &gfx) override;
  void cleanup() override;

  const char *getName() override;

private:
  uint32_t interval = 2500;
  uint32_t lastTime = 0;

  PlayerStatManager *statManager;
  uint16_t lastScore;
};
