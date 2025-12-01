#include "minigames/EndScreen.h"
#include "PlayerStatManager.h"
#include "minigames/StartMenu.h"

EndScreen::EndScreen(PlayerStatManager *statManager, uint16_t newScore)
    : statManager(statManager), lastScore(newScore) {}

void EndScreen::init(Arduino_GFX &gfx) {
  gfx.fillScreen(RGB565_BLACK);
  gfx.setTextColor(RGB565_WHITE, RGB565_BLACK);
  gfx.setTextSize(2);
  lastTime = millis();

  int y = 20;
  int x = 70;
  gfx.setCursor(x, y);
  gfx.print("LAST SCORE: ");
  gfx.print(lastScore);
  y += 40;

  gfx.setCursor(x, y);
  gfx.print("TOP SCORES");
  y += 40;

  const PlayerStat *stats = statManager->getStats();
  for (uint8_t i = 0; i < MAX_PLAYER_COUNT; i++) {
    gfx.setCursor(x, y);
    gfx.print(i + 1);
    gfx.print(": ");
    gfx.print(stats[i].score);
    y += 20;
  }
}

void EndScreen::update(uint32_t deltaTime, Keyboard &keyboard, Joystick &joystick){
  uint32_t currentTime = millis();
  if (currentTime - lastTime >= interval) {
    gameComplete = true;
  }
}

void EndScreen::render(uint32_t deltaTime, Arduino_GFX &gfx) {

}

void EndScreen::cleanup() {
  // ei tarvitse tehdä mitään täällä
}

const char *EndScreen::getName() { return "EndScreen"; }
