#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include <Arduino_GFX.h>
#include <Joystick.h>
#include <Keyboard.h>

class Game {
public:
  virtual ~Game() {}

  virtual void init() = 0;

  // deltaTime on millisekuntteina viime päivityksestä
  virtual void update(uint32_t deltaTime, Keyboard &keyboard,
                      Joystick &Joystick) = 0;

  virtual void render(uint32_t deltaTime, Arduino_GFX &gfx);

  virtual void cleanup() = 0;

  virtual bool isComplete() = 0;

  virtual const char *getName() { return ""; }
};

#endif
