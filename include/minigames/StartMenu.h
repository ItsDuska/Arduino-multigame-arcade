#pragma once
#include "GameInterface.h"

class GameManager;
class PlayerStatManager;

class StartMenu : public Game {
public:
  StartMenu(GameManager *gameManager, PlayerStatManager *playerStatManager);
  void init(Arduino_GFX &gfx) override;
  void update(uint32_t deltaTime, Keyboard &keyboard,
              Joystick &Joystick) override;
  void render(uint32_t deltaTime, Arduino_GFX &gfx) override;
  void cleanup() override;

  const char *getName() override;

private:
  uint8_t currentLineIndex = 0;
  GameManager *gameManager;
  PlayerStatManager *playerStatManager;

  bool isDirty = true;
  uint32_t lastInputTime = 0;
  const uint32_t INPUT_DELAY = 200;
};
