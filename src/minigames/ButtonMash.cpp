#include "utility.h"
#include <minigames/ButtonMash.h>

void ButtonMashGame::cleanup() {}

const char *ButtonMashGame::getName() { return "Button Mash"; }

ButtonMashGame::ButtonMashGame()
    : currentMillis(0), previousMillis(0), countdown(3), state(COUNTDOWN),
      keyPresses(0), initialRender(false) {}

void ButtonMashGame::init(Arduino_GFX &gfx) {
  gfx.fillScreen(RGB565_BLACK);
  gfx.setTextColor(RGB565_WHITE);
  gfx.setTextSize(TEXT_SIZE);
  previousMillis = millis();
}

void ButtonMashGame::update(Keyboard &keyboard, Joystick &Joystick) {
  currentMillis = millis();
  switch (state) {
  case COUNTDOWN: {
    if (countdown > 0) {
      if (currentMillis - previousMillis >= 1000) {
        previousMillis += 1000;
        countdown--;
        isDirty = true;
        if (countdown == 0) {
          previousMillis = currentMillis;
        }
      }
    } else {
      if (currentMillis - previousMillis >= 1000) {
        state = PLAYING;
        previousMillis = currentMillis;
        isDirty = true;
      }
    }
    break;
  }

  case PLAYING: {
    // Consume keyboard events and count presses
    keyboard.update();
    while (keyboard.hasEvent()) {
      Keyboard::KeyEvent ev = keyboard.nextEvent();
      if (ev.type == Keyboard::KeyEvent::Type::PRESS) {
        if (keyPresses < 30) {
          keyPresses++;
          isDirty = true;
        }
      }
    }

    // End conditions: either reached 30 or ran out of time
    const bool success = (keyPresses >= 30);
    const bool timeUp = (currentMillis - previousMillis >= GAME_DURATION);
    if (success || timeUp) {
      hasWon = success;
      state = GAMEOVER;
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

void ButtonMashGame::render(Arduino_GFX &gfx) {
  if (!isDirty)
    return;

  isDirty = false; // resetoidaan lippu
  switch (state) {
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
    // Piirretään otsikko vain kerran alussa, jotta se ei vilku
    if (!initialRender) {
      gfx.fillScreen(RGB565_BLACK);
      gfx.setTextColor(RGB565_WHITE);
      // Nostetaan otsikkoa vähän ylemmäs (-60) ettei se ole numeroiden tiellä
      utilityPrintCenter(gfx, "Paina nappeja...", 2, 0, -60);
      initialRender = true;
    }

    // --- PÄIVITYSLOGIIKKA ---

    // 1. Rakennetaan teksti: "NYKYINEN / TAVOITE"
    // Huom: Tavoite on kovakoodattu 30, koska update-loopissa oli raja 30.
    String statusText = String(keyPresses) + "/30";

    // 2. Määrittele pyyhittävä alue keskellä ruutua.
    // TEXT_SIZE 4 on n. 32px korkea. "30/30" on n. 120px leveä.
    // Otetaan reilu alue varmuuden vuoksi.
    int clearW = 200;
    int clearH = 50;
    int clearX = (gfx.width() - clearW) / 2;
    int clearY = (gfx.height() - clearH) / 2;

    // 3. Piirrä musta laatikko tekstin alle (pyyhkii vanhan)
    gfx.fillRect(clearX, clearY, clearW, clearH, RGB565_BLACK);

    // 4. Piirrä uusi yhdistetty teksti
    gfx.setTextColor(RGB565_WHITE);
    utilityPrintCenter(gfx, statusText.c_str(), TEXT_SIZE, 0, 0);
    break;
  }

  case GAMEOVER:
    gfx.fillScreen(RGB565_BLACK);
    utilityPrintCenter(gfx, "No morjens!", TEXT_SIZE, 0, 0);
    break;
  }
}
