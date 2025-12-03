#include "minigames/LevelSelector.h"
#include "GameManager.h"

constexpr char gameName[] = "LEVEL SELECTOR";
constexpr uint8_t MENU_NAME_LEN = sizeof(gameName) - 1;

constexpr const char *gameStrings[] = {
    "Falling Blocks", "Reaction Game", "Maze Game", "Button Smash",
    "Kalastus", // Stardew valley fishig minigame
    "Muisti Peli",    "Dino Peli"};

constexpr uint8_t MENU_STRINGS_COUNT =
    sizeof(gameStrings) / sizeof(gameStrings[0]);

LevelSelector::LevelSelector(GameManager *gameManager)
    : currentLineIndex(0), gameManager(gameManager) {}

void LevelSelector::init(Arduino_GFX &gfx) {
  gfx.setTextColor(RGB565_WHITE, RGB565_BLACK);
  gfx.setTextSize(2);
}

void LevelSelector::update(uint32_t deltaTime, Keyboard &keyboard,
                           Joystick &joystick) {

  while (keyboard.hasEvent()) {
    Keyboard::KeyEvent ev = keyboard.nextEvent();
    if (ev.type == Keyboard::KeyEvent::Type::PRESS) {
      if (ev.key == '*') {

        // WARNING: Muista laittaa GameRegisterissä pelit oikeeseen kohtaan.
        // Haluamme että tällä sais helposti pelin indexillä pelin pyöriin.
        gameManager->overrideGameIndex(
            currentLineIndex); // mahdollinen offset tähän jos tarvii.
        gameComplete = true;
      } else if (ev.key == '0') {
        gameManager->overrideGameIndex(0, true);
      }
    }
  }

  joystick.update();
  joystick.getPosition();
  Joystick::Direction dir = joystick.convertPositionToDirection();

  uint32_t currentTime = millis();

  if (currentTime - lastInputTime > INPUT_DELAY) {
    bool moved = false;

    switch (dir) {
    case Joystick::Direction::UP:
      currentLineIndex =
          (currentLineIndex + MENU_STRINGS_COUNT - 1) % MENU_STRINGS_COUNT;
      moved = true;
      break;
    case Joystick::Direction::DOWN:
      currentLineIndex = (currentLineIndex + 1) % MENU_STRINGS_COUNT;
      moved = true;
      break;
    default:
      break;
    }

    if (moved) {
      isDirty = true;
      lastInputTime = currentTime;
    }
  }
}

void LevelSelector::render(uint32_t deltaTime, Arduino_GFX &gfx) {
  if (!isDirty) {
    return;
  }
  isDirty = false;

  const int screenWidth = gfx.width();

  constexpr int textScale = 2;
  constexpr int charWidth = 6 * textScale;
  constexpr int charHeight = 8 * textScale;
  constexpr int lineSpacing = 6;

  const int titleWidth = MENU_NAME_LEN * charWidth;
  const int titleX = (screenWidth - titleWidth) / 2;
  const int titleY = 40;

  gfx.setTextColor(RGB565_WHITE, RGB565_WHITE);
  gfx.setCursor(titleX, titleY);
  gfx.print(gameName);

  const int startY = titleY * 2;
  for (uint8_t i = 0; i < MENU_STRINGS_COUNT; i++) {
    const int strWidth =
        sizeof(*gameStrings[i]) == 0 ? 0 : strlen(gameStrings[i]) * charWidth;
    const int x = (screenWidth - strWidth) / 2;
    const int y = startY + i * (charHeight + lineSpacing);

    gfx.setCursor(x, y);
    if (i == currentLineIndex) {
      gfx.setTextColor(RGB565_WHITE, RGB565_BLUE);
    } else {
      gfx.setTextColor(RGB565_WHITE, RGB565_BLACK);
    }
    gfx.print(gameStrings[i]);
  }
}

void LevelSelector::cleanup() {
  // ei tarvitse tehdä mitään täällä
}

const char *LevelSelector::getName() { return gameName; }
