#include <minigames/ReactionTime.h>

static volatile bool timerFired = false;
static int countdownCounter = 3;

void reactionTimerISR() { timerFired = true; }

ReactionTimeGame::ReactionTimeGame()
    : phase(START), phaseStartTime(0), reactionTime(0), speedDisplayed(false) {}

void ReactionTimeGame::init(Arduino_GFX &gfx) {
  gfx.fillScreen(RGB565_BLACK);
  gfx.setTextColor(RGB565_WHITE);
  gfx.setTextSize(2);

  timerFired = false;
  countdownCounter = 3;
  testCount = 0;
  gameComplete = false;
  phase = START;

  // Käynnistetään ajastin: 1 sekunti (1 000 000 mikrosekuntia)
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(reactionTimerISR);
};

void ReactionTimeGame::update(uint32_t deltaTime, Keyboard &keyboard,
                              Joystick &joystick) {
  if (gameComplete)
    return;

  switch (phase) {
  case START:
    // Odotetaan että TimerOne laukaisee keskeytyksen
    if (timerFired) {
      timerFired = false;
      countdownCounter--;

      if (countdownCounter <= 0) {
        phase = WAITING;
        Timer1.stop();

        // Arvotaan satunnainen viive (2-5 sekuntia) ja käynnistetään ajastin
        // uudelleen
        uint32_t randomDelay = random(2000, 5000) * 1000;
        Timer1.setPeriod(randomDelay);
        Timer1.restart();
      }
    }
    break;

  case WAITING:
    // Odotetaan satunnaisen ajan kulumista
    if (timerFired) {
      timerFired = false;
      Timer1.stop(); // Pysäytetään TimerOne, mittaus alkaa nyt

      phaseStartTime = millis();
      phase = REACTSCREEN;
    }
    break;

  case REACTSCREEN:
    // Odotetaan pelaajan syötettä
    while (keyboard.hasEvent()) {
      Keyboard::KeyEvent ev = keyboard.nextEvent();
      if (ev.type == Keyboard::KeyEvent::Type::PRESS) {
        reactionTime = millis() - phaseStartTime;
        phase = SPEED_DISPLAY;
        speedDisplayed = false;
        break;
      }
    }
    break;

  case SPEED_DISPLAY:
    while (keyboard.hasEvent()) {
      Keyboard::KeyEvent ev = keyboard.nextEvent();
      if (ev.type == Keyboard::KeyEvent::Type::PRESS) {
        testCount++;

        if (testCount < TEST_ROUNDS) {
          // Uusi kierros
          phase = WAITING;
          timerFired = false;

          uint32_t randomDelay = random(2000, 5000) * 1000;
          Timer1.setPeriod(randomDelay);
          Timer1.restart();
        } else {
          phase = COMPLETE;
        }
        break;
      }
    }
    break;

  case COMPLETE:
    gameComplete = true;
    break;

  default:
    break;
  }
}

void ReactionTimeGame::render(uint32_t deltaTime, Arduino_GFX &gfx) {
  const int screenWidth = gfx.width();
  const int titleX = 50;
  const int titleY = 40;

  switch (phase) {
  case START:
    gfx.fillScreen(RGB565_BLACK);
    gfx.setCursor(titleX, titleY);
    gfx.print("Valmistaudu...");

    // Piirretään laskuri
    gfx.setTextSize(4);
    gfx.setCursor(screenWidth / 2 - 10, titleY * 2 + 40);
    gfx.print(countdownCounter);
    gfx.setTextSize(2);
    break;

  case WAITING:
    gfx.fillScreen(RGB565_BLACK);
    break;

  case REACTSCREEN:
    gfx.drawCircle(160, 120, 50, RGB565_CYAN);
    break;

  case SPEED_DISPLAY:
    if (!speedDisplayed) {
      gfx.fillScreen(RGB565_BLACK);
      gfx.setTextColor(RGB565_WHITE);
      gfx.setCursor(titleX, titleY + 50);
      gfx.print("Reaktioaika: ");
      gfx.print(reactionTime);
      gfx.print(" ms");
      speedDisplayed = true;
    }
    break;

  case COMPLETE:
    gfx.fillScreen(RGB565_BLACK);
    gfx.setCursor(titleX, titleY);
    gfx.print("Peli ohi!");
    break;
  }
}

void ReactionTimeGame::cleanup() {
  Timer1.detachInterrupt();
  Timer1.stop();
}

const char *ReactionTimeGame::getName() { return "Reaction Time"; }
