#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "minigames/GameInterface.h"
#include "minigames/GameRegistry.h"

#include <Joystick.h>
#include <Keyboard.h>

// BEHOLD! Kauhee määrä includeja, joista en oo vielä jaksanu katsoa että mikkä
// on turhia ja mikkä eivät. jotenka...
// TODO: FIX ME
#include <Arduino_DataBus.h>
#include <Arduino_G.h>
#include <Arduino_GFX.h>
#include <Arduino_GFX_Library.h>
#include <Arduino_OLED.h>
#include <Arduino_TFT.h>
#include <Arduino_TFT_18bit.h>
#include <YCbCr2RGB.h>
#include <gfxfont.h>

enum class GameState : char {
  STATE_MENU,
  STATE_GAME_INIT,
  STATE_GAME_RUNNING,
  STATE_GAME_OVER,
  STATE_ALL_COMPLETE
};

class GameManager {

public:
  GameManager();
  ~GameManager();

  void init();

  void update();

private:
  void initNextGame();

  void cleanupCurrentGame();

private:
  GameState currentState;
  uint8_t currentGameIndex;
  uint8_t totalGames;
  Game *activeGame;
  uint32_t lastUpdateTime;

  Arduino_DataBus *bus;
  Arduino_GFX *gfx;

  Keyboard keyboard;
  Joystick Joystick;
};

#endif
