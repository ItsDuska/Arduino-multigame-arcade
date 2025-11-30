#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include <HardwareWrapper.h>
#include <Joystick.h>
#include <Keyboard.h>

struct GameStats {
  uint16_t score = 0;
  bool gameStatus = false; // win or a loss
  uint8_t flags; // voi käyttää tarvittaessa johonkin. Muuten tällä ei ole väliä
};

class Game {
public:
  virtual ~Game() = default;

  virtual void init(Arduino_GFX &gfx) = 0;

  // deltaTime on millisekuntteina viime päivityksestä
  virtual void update(uint32_t deltaTime, Keyboard &keyboard,
                      Joystick &Joystick) = 0;

  virtual void render(uint32_t deltaTime, Arduino_GFX &gfx) = 0;

  virtual void cleanup() = 0;

  bool isComplete() const { return gameComplete; }

  GameStats getGameStatus() const { return gameStats; }

  virtual const char *getName() { return ""; }

protected:
  GameStats gameStats;
  bool gameComplete = false;
};

#endif
