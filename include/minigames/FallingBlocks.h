#pragma once
#include "GameInterface.h"

constexpr uint8_t MAX_OBSTACLES = 12;

struct Obstacle {
  int8_t x;
  int8_t y;
  bool active;
};

class FallingBlocks : public Game {
public:
  FallingBlocks();
  void init(Arduino_GFX &gfx) override;
  void update(uint32_t deltaTime, Keyboard &keyboard,
              Joystick &Joystick) override;
  void render(uint32_t deltaTime, Arduino_GFX &gfx) override;
  void cleanup() override;
  const char *getName() override;

private:
  void spawnObstacle();
  void moveObstacles();
  void checkCollision();
  void handlePlayerInput(Keyboard &keyboard, Joystick &joystick);
  void adjustDifficulty();

private:
  Obstacle obstacles[MAX_OBSTACLES];
  uint32_t lastSpawnTime;
  uint32_t lastMoveTime;
  uint16_t score;
  uint8_t playerX;
  uint16_t speed;
  uint32_t spawnInterval;
  uint16_t gamesPlayed = 1;
};
