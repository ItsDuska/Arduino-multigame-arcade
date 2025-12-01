#include "GameManager.h"
#include "minigames/EndScreen.h"
#include "minigames/LevelSelector.h"
#include "minigames/StartMenu.h"

#define PIN_RD A6
#define PIN_WR A5
#define PIN_RS A4
#define PIN_CS A3
#define PIN_RST A7

#define JOYSTICK_PIN_A 0
#define JOYSTICK_PIN_B 1
#define JOYSTICK_PIN_C 2
#define MAIN_MENU_GAME_INDEX 0

GameManager::GameManager()
    : currentState(GameState::STATE_MENU), currentGameIndex(0),
      totalGames(GAME_COUNT), activeGame(nullptr), lastUpdateTime(0),
      joystick(JOYSTICK_PIN_A, JOYSTICK_PIN_B, JOYSTICK_PIN_C),
      playerStatManager(0) {}

GameManager::~GameManager() {
  delete gfx;
  delete bus;
}

void GameManager::init() {
  bus = new Arduino_SWPAR8(PIN_RS, PIN_CS, PIN_WR, PIN_RD, A8, A9, A10, A11,
                           A12, A13, A14, A15);
  gfx = new Arduino_ILI9341(bus, PIN_RST, 0, false);

  playerStatManager.read();
  gfx->begin();
  gfx->setRotation(1);
  gfx->fillScreen(RGB565_BLACK);
  gfx->setRotation(3);

  Serial.println("=== Multi-Game Arcade ===");
  Serial.print("Total games: ");
  Serial.println(totalGames);

  currentState = GameState::STATE_MENU;
}

void GameManager::update() {
  uint32_t currentTime = millis();
  uint32_t deltaTime = currentTime - lastUpdateTime;
  lastUpdateTime = currentTime;

  keyboard.update();
  joystick.update();

  switch (currentState) {
  case GameState::STATE_MENU:
    gfx->fillScreen(RGB565_BLACK);
    activeGame = new StartMenu(this, &playerStatManager);
    activeGame->init(*gfx);
    lostGameCount = 0;
    currentState = GameState::STATE_GAME_RUNNING;
    break;
    case GameState::STATE_LEVEL_SELECT:
      gfx->fillScreen(RGB565_BLACK);
      activeGame = new LevelSelector(this);
      activeGame->init(*gfx);
      currentState = GameState::STATE_GAME_RUNNING;
      break;


  case GameState::STATE_GAME_INIT:
    initNextGame();
    break;

  case GameState::STATE_GAME_RUNNING:
    processActiveGameFrame(deltaTime);
    if (overrideState != GameState::STATE_NULL)
    {
      currentState = overrideState;
      overrideState = GameState::STATE_NULL;
    }
    break;

  case GameState::STATE_GAME_OVER:
    Serial.print("GAME OVER\n");
    if (overrideState != GameState::STATE_NULL)
    {
      currentState = overrideState;
    }
    else{
      currentState = GameState::STATE_GAME_INIT;
    }
    updateScore();
    cleanupCurrentGame();

    break;

  case GameState::STATE_ALL_COMPLETE:
    if (!activeGame) {
      playerStatManager.add(currentScore);
      playerStatManager.save();
      activeGame = new EndScreen(&playerStatManager, currentScore);
      activeGame->init(*gfx);
    }

    processActiveGameFrame(deltaTime);

    if (activeGame->isComplete()) {
      cleanupCurrentGame();
      currentState = GameState::STATE_MENU;
    }

    break;
  }
}

void GameManager::initNextGame(uint8_t gameIndex) {
  Serial.print("Loading game ");
  Serial.print(currentGameIndex + 1);
  Serial.print("/");
  Serial.println(totalGames);

  if (_overrideGameIndex) {
    gameIndex = currentGameIndex;
  } else if (gameIndex == RANDOM_GAME_FLAG) {
    gameIndex = random(0, totalGames);
  }
  activeGame = createGame(gameIndex);

  if (activeGame) {
    activeGame->init(*gfx);
    currentState = GameState::STATE_GAME_RUNNING;
    const char *gameName = activeGame->getName();
    if (gameName) {
      Serial.print("Game name: ");
      Serial.println(gameName);
    }
  } else {
    Serial.println("ERROR: Failed to create game");
    currentState = GameState::STATE_ALL_COMPLETE;
  }
}

void GameManager::cleanupCurrentGame() {
  if (activeGame) {
    activeGame->cleanup();
    delete activeGame;
    activeGame = nullptr;
  }
}

void GameManager::updateScore() {
  if (activeGame) {
    GameStats stats = activeGame->getGameStatus();
    currentScore += stats.score;

    if (!stats.gameStatus) {
      lostGameCount++;
      if (lostGameCount >= maxLostGames) {
        Serial.println("Maximum lost games reached");
        // n määrä hävitty joten peli on kokonaan ohi. Mene takaisin Main Menuun
        currentState = GameState::STATE_ALL_COMPLETE;
      }
    }
  }
}

void GameManager::overrideGameIndex(uint8_t gameIndex, bool isMenu) {
  if (isMenu)
  {
    Serial.println(gameIndex);
    Serial.print("aaaaaa");
    switch(gameIndex)
    {
    case 1:
      overrideState = GameState::STATE_LEVEL_SELECT;
      Serial.print("amogus");
    break;
    default:
      break;
    } // tähän voi lisää muita menuja jos niitä tulee.
  }
  else {
    overrideState = GameState::STATE_NULL;
  }

  currentGameIndex = gameIndex;
  _overrideGameIndex = true;
}

void GameManager::processActiveGameFrame(uint32_t deltaTime) {
  if (!activeGame) {
    return;
  }

  activeGame->update(deltaTime, keyboard, joystick);
  activeGame->render(deltaTime, *gfx);

  if (activeGame->isComplete()) {
    if (overrideState != GameState::STATE_NULL) {
      currentState = overrideState;
      overrideState = GameState::STATE_NULL; // <- nollaus tärkeä!
    } else {
      currentState = GameState::STATE_GAME_OVER;
    }
  }
}

void GameManager::setScore(uint16_t score) {
  currentScore = score;
}
