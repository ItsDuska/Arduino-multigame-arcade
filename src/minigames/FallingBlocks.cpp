#include <minigames/FallingBlocks.h>

FallingBlocks::FallingBlocks() {}

void FallingBlocks::init() {}

void FallingBlocks::update(uint32_t deltaTime, Keyboard &keyboard,
                           Joystick &joystick) {
  if (gameOver) {
    return;
  }

  handlePlayerInput(keyboard, joystick);

  const auto currentTime = millis();

  if (currentTime - lastSpawnTime >= SPAWN_INTERVAL) {
    spawnObstacle();
    lastSpawnTime = millis();
  }

  if (currentTime - lastMoveTime >= 200) {
    moveObstacles();
    lastMoveTime = millis();
  }

  checkCollision();
}

void FallingBlocks::render(uint32_t deltaTime, Arduino_GFX &gfx) {}

void FallingBlocks::cleanup() {}

bool FallingBlocks::isComplete() { return false; }

const char *FallingBlocks::getName() { return nullptr; }

void FallingBlocks::spawnObstacle() {
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (!obstacles[i].active) {
      obstacles[i].active = true;
      obstacles[i].y = 0;
      obstacles[i].x = random(0, COLS);
      break;
    }
  }
}

void FallingBlocks::moveObstacles() {
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (!obstacles[i].active) {
      continue;
    }

    obstacles[i].y++;

    if (obstacles[i].y >= ROWS) {
      obstacles[i].active = false;
      score++;
    }
  }
}

void FallingBlocks::checkCollision() {
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (!obstacles[i].active) {
      continue;
    }

    if (obstacles[i].y == ROWS - 1 && obstacles[i].x == playerX) {
      gameOver = true;
      return;
    }
  }
}

void FallingBlocks::handlePlayerInput(Keyboard &keyboard, Joystick &joystick) {
  while (keyboard.hasEvent()) {
    Keyboard::KeyEvent ev = keyboard.nextEvent();
    if (ev.type == Keyboard::KeyEvent::Type::PRESS) {
      if (ev.key == 'A' && playerX > 0) {
        playerX--;
      } else if (ev.key == 'D' && playerX < COLS - 1) {
        playerX++;
      }
    }
  }

  // mieluiten joystick.readPositions();
  // sitten getPostion();
  Vec2u16 position = joystick.readPosition();
  Joystick::Direction dir = joystick.convertPositionToDirection();

  switch (dir) {
  case Joystick::Direction::LEFT:
    if (playerX > 0) {
      playerX--;
    }
    break;
  case Joystick::Direction::RIGHT:
    if (playerX < COLS - 1) {
      playerX--;
    }
    break;

  default:
    break;
  }
  // lisää joystick support tähän
}
