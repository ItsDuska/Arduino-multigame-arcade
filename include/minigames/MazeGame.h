#pragma once
#include "GameInterface.h"
#include "Vec2.h"

enum class CellType : uint8_t { AIR = 0, BLOCK = 1, START = 2, END = 3 };

class MazeGame : public Game {
public:
  MazeGame(uint8_t width = 15, uint8_t height = 15);
  ~MazeGame();

  void init(Arduino_GFX &gfx) override;
  void update(uint32_t deltaTime, Keyboard &keyboard,
              Joystick &joystick) override;
  void render(uint32_t deltaTime, Arduino_GFX &gfx) override;
  void cleanup() override;

  const char *getName() override;

private:
  void generateMaze();
  void carvePath(u8Vec2 position);
  void placeStartAndEnd();
  bool isValidCell(u8Vec2 position) const;
  CellType getCell(u8Vec2 position) const;
  CellType getCell(uint8_t x, uint8_t y) const;
  void setCell(u8Vec2 position, CellType type);
  void drawMaze(Arduino_GFX &gfx);

private:
  u8Vec2 mazeSize;
  CellType *maze;

  u8Vec2 player;
  u8Vec2 oldPlayer;
  u8Vec2 start;
  u8Vec2 end;
  bool mazeDrawn;
};
