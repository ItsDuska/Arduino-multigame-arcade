#include <minigames/ButtonMash.h>
#include "utility.h"

void ButtonMashGame::cleanup() {}

const char *ButtonMashGame::getName() { return "Button Mash"; }

ButtonMashGame::ButtonMashGame()
    : currentMillis(0), previousMillis(0), countdown(3),
    state(COUNTDOWN), keyPresses(0) {}

void ButtonMashGame::init(Arduino_GFX &gfx) {
  gfx.fillScreen(RGB565_BLACK);
  gfx.setTextColor(RGB565_WHITE);
  gfx.setTextSize(TEXT_SIZE);
}

void ButtonMashGame::update(uint32_t deltaTime, Keyboard &keyboard, Joystick &Joystick) {
  switch (state) {
    case COUNTDOWN: {
      currentMillis += deltaTime;
      if (countdown > 0) {
        if (currentMillis - previousMillis >= 1000) {
            previousMillis += 1000;
            countdown--;
            if (countdown == 0) {
                previousMillis = currentMillis;
            }
        }
      } else {
        if (currentMillis - previousMillis >= 1000) {
          state = PLAYING;
        }
      }
      break;
    }

    case PLAYING: {
      // Accumulate elapsed time in the playing phase
      currentMillis += deltaTime;

      // Consume keyboard events and count presses
      while (keyboard.hasEvent()) {
      Keyboard::KeyEvent ev = keyboard.nextEvent();
      if (ev.type == Keyboard::KeyEvent::Type::PRESS) {
          if (keyPresses < 30) {
          keyPresses++;
          }
      }
      }

      // End conditions: either reached 30 or ran out of time
      const bool success = (keyPresses >= 30);
      const bool timeUp = (currentMillis >= GAME_DURATION);
      if (success || timeUp) {
        state = GAMEOVER;
        // Report results
        gameStats.score = keyPresses;
        gameStats.gameStatus = success;
      }
      break;
    }

    case GAMEOVER:
      gameComplete = true;
      break;

    default:
      break;
  }
}

void ButtonMashGame::render(uint32_t deltaTime, Arduino_GFX &gfx) {
  switch(state) {
    case COUNTDOWN: {
      gfx.fillScreen(RGB565_BLACK);
      gfx.setTextColor(RGB565_WHITE);

      if (countdown > 0) {
        char buf[8];
        snprintf(buf, sizeof(buf), "%u", countdown);
        utilityPrintCenter(gfx, buf, TEXT_SIZE, 0, 0);
      } else {
          utilityPrintCenter(gfx, "MASH!", TEXT_SIZE, 0, 0);
      }
      break;
    }

    case PLAYING: {
      gfx.fillScreen(RGB565_BLACK);
      gfx.setTextColor(RGB565_WHITE);

      char buf[16];
      snprintf(buf, sizeof(buf), "%u/30", (unsigned)keyPresses);
      utilityPrintCenter(gfx, "Paina nappeja...", 2, 0, -50);
      utilityPrintCenter(gfx, buf, TEXT_SIZE, 0, 0);
      break;
    }

    case GAMEOVER:
      gfx.fillScreen(RGB565_BLACK);
      utilityPrintCenter(gfx, "No morjens!", TEXT_SIZE, 0, 0);
      break;
  }
}
