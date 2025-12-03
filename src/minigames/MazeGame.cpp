#include "Vec2.h"
#include <cstdint>
#include <minigames/MazeGame.h>

constexpr uint16_t COLOR_BG = RGB565_BLACK;
constexpr uint16_t COLOR_WALL = RGB565_BLUE;
constexpr uint16_t COLOR_PLAYER = RGB565_GREEN;
constexpr uint16_t COLOR_START = RGB565_YELLOW;
constexpr uint16_t COLOR_END = RGB565_RED;
constexpr uint16_t COLOR_PATH = COLOR_BG;

MazeGame::MazeGame(uint8_t width, uint8_t height)
    : mazeSize(width, height), maze(nullptr), player(0, 0), oldPlayer(0, 0),
      start(0, 0), end(0, 0), mazeDrawn(false) {
  maze = new CellType[mazeSize.width * mazeSize.height];
}

MazeGame::~MazeGame() {
  if (maze) {
    delete[] maze;
    maze = nullptr;
  }
}

void MazeGame::init(Arduino_GFX &gfx) {
  gfx.fillScreen(COLOR_BG);
  generateMaze();

  player.x = start.x;
  player.y = start.y;
  oldPlayer = player;

  gameComplete = false;
  drawMaze(gfx);
}

void MazeGame::update(uint32_t deltaTime, Keyboard &keyboard,
                      Joystick &joystick) {
  if (gameComplete)
    return;

  oldPlayer = player;

  while (keyboard.hasEvent()) {
    Keyboard::KeyEvent ev = keyboard.nextEvent();
    if (ev.type == Keyboard::KeyEvent::Type::PRESS) {
      switch (ev.key) {
      case '7': // Left | A KEY
        if (player.x > 0 && getCell(player.x - 1, player.y) != CellType::BLOCK)
          player.x--;
        break;
      case '9': // Right | D KEY
        if (player.x < mazeSize.width - 1 &&
            getCell(player.x + 1, player.y) != CellType::BLOCK)
          player.x++;
        break;
      case '5': // Up | W KEY
        if (player.y > 0 && getCell(player.x, player.y - 1) != CellType::BLOCK)
          player.y--;
        break;
      case '8': // Down | S KEY
        if (player.y < mazeSize.height - 1 &&
            getCell(player.x, player.y + 1) != CellType::BLOCK)
          player.y++;
        break;
      }
    }
  }

  joystick.update();
  joystick.getPosition();
  Joystick::Direction dir = joystick.convertPositionToDirection();

  switch (dir) {
  case Joystick::Direction::LEFT:
    if (player.x > 0 && getCell(player.x - 1, player.y) != CellType::BLOCK)
      player.x--;
    break;
  case Joystick::Direction::RIGHT:
    if (player.x < mazeSize.width - 1 &&
        getCell(player.x + 1, player.y) != CellType::BLOCK)
      player.x++;
    break;
  case Joystick::Direction::UP:
    if (player.y > 0 && getCell(player.x, player.y - 1) != CellType::BLOCK)
      player.y--;
    break;
  case Joystick::Direction::DOWN:
    if (player.y < mazeSize.height - 1 &&
        getCell(player.x, player.y + 1) != CellType::BLOCK)
      player.y++;
    break;
  default:
    break;
  }

  if (player.x == end.x && player.y == end.y) {
    gameComplete = true;
    gameStats.gameStatus = true; // Win
    gameStats.score = 100;
  }
}

void MazeGame::render(uint32_t deltaTime, Arduino_GFX &gfx) {
  i16Vec2 cellSize(gfx.width() / mazeSize.width,
                   gfx.height() / mazeSize.height);

  // poista vanha pelaaja.
  gfx.fillRect(oldPlayer.x * cellSize.width, oldPlayer.y * cellSize.height,
               cellSize.width, cellSize.height, COLOR_BG);

  // piirrä uus pelaaja.
  gfx.drawRect(player.x * cellSize.width, player.y * cellSize.height,
               cellSize.width, cellSize.height, COLOR_PLAYER);
}

void MazeGame::drawMaze(Arduino_GFX &gfx) {
  i16Vec2 cellSize(gfx.width() / mazeSize.width,
                   gfx.height() / mazeSize.height);

  for (uint8_t y = 0; y < mazeSize.height; y++) {
    for (uint8_t x = 0; x < mazeSize.width; x++) {
      uint16_t color;
      CellType cell = getCell(x, y);

      switch (cell) {
      case CellType::BLOCK:
        color = COLOR_WALL;
        break;
      case CellType::START:
        color = COLOR_START;
        break;
      case CellType::END:
        color = COLOR_END;
        break;
      case CellType::AIR:
      default:
        color = COLOR_PATH;
        break;
      }

      gfx.fillRect(x * cellSize.width, y * cellSize.height, cellSize.width,
                   cellSize.height, color);
    }
  }
  mazeDrawn = true;
}

void MazeGame::cleanup() {}

const char *MazeGame::getName() { return "Maze Game"; }

void MazeGame::generateMaze() {
  u8Vec2 pos(0, 0);
  for (pos.y = 0; pos.y < mazeSize.height; pos.y++) {
    for (pos.x = 0; pos.x < mazeSize.width; pos.x++) {
      setCell(pos, CellType::BLOCK);
    }
  }
  u8Vec2 startCell(0, 0);
  startCell.x = 1 + (random(0, (mazeSize.width - 1) / 2) * 2);
  startCell.y = 1 + (random(0, (mazeSize.height - 1) / 2) * 2);

  carvePath(startCell);

  placeStartAndEnd();
}

void MazeGame::carvePath(u8Vec2 position) {
  setCell(position, CellType::AIR);

  i8Vec2 direction[] = {{0, -2}, {2, 0}, {0, 2}, {-2, 0}};

  uint8_t order[] = {0, 1, 2, 3};
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t j = random(i, 4);
    uint8_t temp = order[i];
    order[i] = order[j];
    order[j] = temp;
  }

  for (uint8_t i = 0; i < 4; i++) {
    uint8_t dir = order[i];
    u8Vec2 newPosition(position.x + direction[dir].x,
                       position.y + direction[dir].y);

    if (newPosition.x > 0 && newPosition.x < mazeSize.width - 1 &&
        newPosition.y > 0 && newPosition.y < mazeSize.height - 1 &&
        getCell(newPosition.x, newPosition.y) == CellType::BLOCK) {

      u8Vec2 airBlock(position.x + direction[dir].x / 2,
                      position.y + direction[dir].y / 2);
      setCell(airBlock, CellType::AIR);

      carvePath(newPosition);
    }
  }
}

void MazeGame::placeStartAndEnd() {
  uint8_t airCells[255][2];
  uint8_t airCount = 0;

  for (uint8_t y = 0; y < mazeSize.height && airCount < 255; y++) {
    for (uint8_t x = 0; x < mazeSize.width && airCount < 255; x++) {
      if (getCell(x, y) == CellType::AIR) {
        airCells[airCount][0] = x;
        airCells[airCount][1] = y;
        airCount++;
      }
    }
  }

  if (airCount < 2) {
    start.x = 1;
    start.y = 1;
    end.x = mazeSize.width - 2;
    end.y = mazeSize.height - 2;
    setCell(start, CellType::START);
    setCell(end, CellType::END);
    return;
  }

  uint8_t startIdx = random(0, airCount);
  start.x = airCells[startIdx][0];
  start.y = airCells[startIdx][1];

  uint8_t maxDistance = 0;
  uint8_t bestEndIdx = 0;

  for (uint8_t i = 0; i < min(airCount, (uint8_t)20u); i++) {
    uint8_t idx = random(0, airCount);
    u8Vec2 direction(0, 0);
    direction.x = abs(airCells[idx][0] - start.x);
    direction.y = abs(airCells[idx][1] - start.y);
    uint8_t distance = direction.x + direction.y;

    if (distance > maxDistance) {
      maxDistance = distance;
      bestEndIdx = idx;
    }
  }

  end.x = airCells[bestEndIdx][0];
  end.y = airCells[bestEndIdx][1];

  setCell(start, CellType::START);
  setCell(end, CellType::END);
}

bool MazeGame::isValidCell(u8Vec2 position) const {
  return position.x < mazeSize.width && position.y < mazeSize.height;
}

CellType MazeGame::getCell(uint8_t x, uint8_t y) const {
  u8Vec2 pos(x, y);
  return getCell(pos);
}

CellType MazeGame::getCell(u8Vec2 position) const {
  if (!isValidCell(position))
    return CellType::BLOCK;
  return maze[position.y * mazeSize.width + position.x];
}

void MazeGame::setCell(u8Vec2 position, CellType type) {
  if (isValidCell(position)) {
    maze[position.y * mazeSize.width + position.x] = type;
  }
}
