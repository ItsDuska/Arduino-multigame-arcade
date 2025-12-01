#include "minigames/StartMenu.h"
#include "GameManager.h"

constexpr char gameName[] = "MAIN MENU";
constexpr uint8_t MENU_NAME_LEN = sizeof(gameName) - 1;

constexpr const char *menuStrings[] = {"Play", "Level Selector", "Stats", "Reset Scores"};
constexpr uint8_t MENU_STRINGS_COUNT = sizeof(menuStrings) / sizeof(menuStrings[0]);

StartMenu::StartMenu(GameManager *gameManager, PlayerStatManager *playerStatManager)
    : currentLineIndex(0), gameManager(gameManager), playerStatManager(playerStatManager){}

void StartMenu::init(Arduino_GFX &gfx) {
    gfx.setTextColor(RGB565_WHITE, RGB565_BLACK);
    gfx.setTextSize(2);
}

void StartMenu::update(uint32_t deltaTime, Keyboard &keyboard, Joystick &joystick) {
    while (keyboard.hasEvent()) {
      Keyboard::KeyEvent ev = keyboard.nextEvent();
      if (ev.type == Keyboard::KeyEvent::Type::PRESS && ev.key == '*') {
        int index = currentLineIndex;
        switch(index) {
          case 0:
              gameManager->overrideGameIndex(index, false);
              break;
          case 1:
              gameManager->overrideGameIndex(index, true);
              break;
          case 2:
            index = 500;
            gameManager->overrideGameIndex(index, true);
            // Heitetään tahalleen error. Tämä vie meidän
            // automaattisesti siihen oikeeseen paikkaan lol.
            break;
          case 3:
              playerStatManager->resetScores();
              gameManager->setScore(0);
              Serial.println("Scores reset");
              break;
          default:
              break;
        }

        gameComplete = true;
        return;
      }
    }

    joystick.update();
    joystick.getPosition();
    Joystick::Direction dir = joystick.convertPositionToDirection();

    switch (dir) {
        case Joystick::Direction::UP:
            currentLineIndex = (currentLineIndex + MENU_STRINGS_COUNT - 1) % MENU_STRINGS_COUNT;
            break;
        case Joystick::Direction::DOWN:
            currentLineIndex = (currentLineIndex + 1) % MENU_STRINGS_COUNT;
            break;
        default:
            break;
    }
}

void StartMenu::render(uint32_t deltaTime, Arduino_GFX &gfx) {
    const int screenWidth = gfx.width();

    constexpr int textScale = 2;
    constexpr int charWidth = 6 * textScale;
    constexpr int charHeight = 8 * textScale;
    constexpr int lineSpacing = 6;

    const int titleWidth = MENU_NAME_LEN * charWidth;
    const int titleX = (screenWidth - titleWidth) / 2;
    const int titleY = 40;

    gfx.setTextColor(RGB565_WHITE, RGB565_BLACK);
    gfx.setCursor(titleX, titleY);
    gfx.print(gameName);

    const int startY = titleY + 40;
    for (uint8_t i = 0; i < MENU_STRINGS_COUNT; i++) {
        const int strWidth = strlen(menuStrings[i]) * charWidth;
        const int x = (screenWidth - strWidth) / 2;
        const int y = startY + i * (charHeight + lineSpacing);

        gfx.setCursor(x, y);
        if (i == currentLineIndex) {
            gfx.setTextColor(RGB565_WHITE, RGB565_BLUE); // valittu rivi sinisellä
        } else {
            gfx.setTextColor(RGB565_WHITE, RGB565_BLACK);
        }
        gfx.print(menuStrings[i]);
    }
}

void StartMenu::cleanup() {
    // Ei tarvitse tehdä mitään
}

const char *StartMenu::getName() {
    return gameName;
}
