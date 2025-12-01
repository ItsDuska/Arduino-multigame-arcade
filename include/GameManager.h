#pragma once
#include <HardwareWrapper.h>

#include "PlayerStatManager.h"
#include "minigames/GameInterface.h"
#include "minigames/GameRegistry.h"

#include <Joystick.h>
#include <Keyboard.h>

#define RANDOM_GAME_FLAG 255

enum class GameState : char {
  STATE_MENU,
  STATE_LEVEL_SELECT,
  STATE_GAME_INIT,
  STATE_GAME_RUNNING,
  STATE_GAME_OVER,
  STATE_ALL_COMPLETE,
  STATE_NULL
};

class GameManager {

public:
  GameManager();
  ~GameManager();

  void init();
  void update();

  void overrideGameIndex(uint8_t gameIndex, bool isMenu = false);
  // käytät tätä scoren resetointii koska jooh.
  void setScore(uint16_t score);

private:
  void processActiveGameFrame(uint32_t deltaTime);
  void initNextGame(uint8_t gameIndex = RANDOM_GAME_FLAG);
  void cleanupCurrentGame();
  void updateScore();

private:
  GameState currentState;
  uint8_t currentGameIndex;
  uint8_t totalGames;
  bool _overrideGameIndex = false;
  GameState overrideState;

  uint16_t currentScore = 0;
  uint8_t lostGameCount = 1;

  uint8_t maxLostGames = 3;
  Game *activeGame;

  uint32_t lastUpdateTime;

  // Nämä tyypit tulevat nyt HardwareWrapper.h:n kautta
  // (Joko oikeina tai Mock-versioina)
  Arduino_DataBus *bus;
  Arduino_GFX *gfx;

  Keyboard keyboard;
  Joystick joystick;

  PlayerStatManager playerStatManager;
};
