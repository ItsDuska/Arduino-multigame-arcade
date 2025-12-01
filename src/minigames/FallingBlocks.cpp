#include <minigames/FallingBlocks.h>

constexpr uint16_t COLOR_BG = RGB565_BLACK;
constexpr uint16_t COLOR_PLAYER = RGB565_GREEN;
constexpr uint16_t COLOR_OBSTACLE = RGB565_RED;

constexpr int ROWS = 12;
constexpr int COLS = 6;

constexpr uint32_t MIN_SPAWN_INTERVAL = 100;
constexpr uint32_t MAX_SPAWN_INTERVAL = 500;

constexpr uint32_t MIN_OBSTACLE_SPEED = 100;
constexpr uint32_t MAX_OBSTACLE_SPEED = 300;

#ifdef TARGET_PC
float min(float a, float b) { return (a < b) ? a : b; }
#endif
FallingBlocks::FallingBlocks()
    : playerX(COLS / 2), lastSpawnTime(0), lastMoveTime(0), score(0) {}

void FallingBlocks::init(Arduino_GFX &gfx) { adjustDifficulty(); }

void FallingBlocks::update(uint32_t deltaTime, Keyboard &keyboard,
                           Joystick &joystick) {
  if (gameComplete)
    return;

  handlePlayerInput(keyboard, joystick);

  const auto currentTime = millis();

  if (currentTime - lastSpawnTime >= spawnInterval) {
    spawnObstacle();
    lastSpawnTime = currentTime;
  }

  if (currentTime - lastMoveTime >= speed) {
    moveObstacles();
    lastMoveTime = currentTime;
  }

  checkCollision();
}

void FallingBlocks::render(uint32_t deltaTime, Arduino_GFX &gfx) {
  gfx.fillScreen(COLOR_BG);

  int cellWidth = gfx.width() / COLS;
  int cellHeight = gfx.height() / ROWS;

  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (obstacles[i].active) {
      gfx.fillRect(obstacles[i].x * cellWidth, obstacles[i].y * cellHeight,
                   cellWidth, cellHeight, COLOR_OBSTACLE);
    }
  }

  gfx.fillRect(playerX * cellWidth, (ROWS - 1) * cellHeight, cellWidth,
               cellHeight, COLOR_PLAYER);

  gfx.setCursor(0, 0);
  gfx.setTextColor(RGB565_WHITE, COLOR_BG);
  gfx.setTextSize(1);
  gfx.print("Score: ");
  gfx.print(score);
}

void FallingBlocks::cleanup() {}

const char *FallingBlocks::getName() { return "Falling Blocks"; }

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
    if (!obstacles[i].active)
      continue;

    obstacles[i].y++;
    if (obstacles[i].y >= ROWS) {
      obstacles[i].active = false;
      score++;
    }
  }
}

void FallingBlocks::checkCollision() {
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (!obstacles[i].active)
      continue;

    if (obstacles[i].y == ROWS - 1 && obstacles[i].x == playerX) {
      gameComplete = true;
      gameStats.gameStatus = false;
      gameStats.score = score;
      return;
    }
  }
}

void FallingBlocks::handlePlayerInput(Keyboard &keyboard, Joystick &joystick) {
  while (keyboard.hasEvent()) {
    Keyboard::KeyEvent ev = keyboard.nextEvent();
    if (ev.type == Keyboard::KeyEvent::Type::PRESS) {
      if (ev.key == '7' && playerX > 0)
        playerX--;
      if (ev.key == '9' && playerX < COLS - 1)
        playerX++;
    }
  }

  joystick.update();
  joystick.getPosition();
  Joystick::Direction dir = joystick.convertPositionToDirection();
  switch (dir) {
  case Joystick::Direction::LEFT:
    if (playerX > 0)
      playerX--;
    break;
  case Joystick::Direction::RIGHT:
    if (playerX < COLS - 1)
      playerX++;
    break;
  default:
    break;
  }
}
// Mitä enemmän pelejä on pelattu, sitä nopeampi peli
void FallingBlocks::adjustDifficulty() {
  int level = gamesPlayed / 5;

  spawnInterval = MAX_SPAWN_INTERVAL - level * 100;
  speed = MAX_OBSTACLE_SPEED - level * 50;

  if (spawnInterval < MIN_SPAWN_INTERVAL)
    spawnInterval = MIN_SPAWN_INTERVAL;
  if (speed < MIN_OBSTACLE_SPEED)
    speed = MIN_OBSTACLE_SPEED;
}
