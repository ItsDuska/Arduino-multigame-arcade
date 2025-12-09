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

  // Häkki jotta nummerolaskuri alkaa kolmosesta koska en jaksa varmistaa
  // renderöinnin toimivuutta
  countdownCounter = 4;
  testCount = 0;
  gameComplete = false;
  phase = START;
  initialRender = false;
  isDirty = true;

  // Käynnistetään ajastin: 1 sekunti (1 000 000 mikrosekuntia)
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(reactionTimerISR);
  timerFired = false;
  Timer1.restart();
};

void ReactionTimeGame::update(Keyboard &keyboard, Joystick &joystick) {
  if (gameComplete)
    return;

  switch (phase) {
  case START:
    // Odotetaan että TimerOne laukaisee keskeytyksen
    if (timerFired) {
      timerFired = false;
      countdownCounter--;
      isDirty = true;

      if (countdownCounter <= 0) {
        phase = WAITING;
        initialRender = false;
        isDirty = true;
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
      initialRender = false;
      isDirty = true;
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
        initialRender = false;
        isDirty = true;
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
          initialRender = false;
          timerFired = false;
          isDirty = true;

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

void ReactionTimeGame::render(Arduino_GFX &gfx) {
  if (!isDirty && !initialRender) {
    return;
  }

  const int screenWidth = gfx.width();
  const int titleX = 50;
  const int titleY = 40;

  switch (phase) {
  case START:
    // Piirrä staattiset tekstit vain kerran
    if (!initialRender) {
      gfx.fillScreen(RGB565_BLACK);
      gfx.setTextColor(RGB565_WHITE);
      gfx.setCursor(titleX + 20, titleY);
      gfx.print("Valmistaudu...");
      gfx.setCursor(titleX - 20, titleY + 30);
      gfx.print("Painettava nappi on *");
      initialRender = true;
    }

    if (isDirty) {
      isDirty = false;
      // Määrittele alue numerolle
      int numX = screenWidth / 2 - 10;
      int numY = titleY * 2 + 40;

      // Pyyhi vanha numero mustalla laatikolla (koko esim 40x40)
      gfx.fillRect(numX - 5, numY - 5, 40, 40, RGB565_BLACK);

      // Piirrä uusi numero
      gfx.setTextSize(4);
      gfx.setCursor(numX, numY);
      gfx.print(countdownCounter);
      gfx.setTextSize(2); // Palauta koko
    }
    break;

  case WAITING:
    if (!initialRender) {
      gfx.fillScreen(RGB565_BLACK); // Tyhjä ruutu odotuksessa
      initialRender = true;
    }
    break;

  case REACTSCREEN:
    if (!initialRender) {
      // Piirretään pallo vain kerran, ei joka frame
      // (paitsi jos haluat sen liikkuvan/vilkkuvan)
      gfx.fillScreen(RGB565_BLACK);
      gfx.drawCircle(160, 120, 50, RGB565_CYAN);
      initialRender = true;
    }
    break;

  case SPEED_DISPLAY:
    if (!initialRender) {
      gfx.fillScreen(RGB565_BLACK);
      gfx.setTextColor(RGB565_WHITE);
      gfx.setCursor(titleX, titleY + 50);
      gfx.print("Reaktioaika: ");
      gfx.print(reactionTime);
      gfx.print(" ms");
      gfx.setCursor(titleX, titleY + 100);
      gfx.print("Paina * jatkaaksesi");
      initialRender = true;
    }
    break;
  }
}

void ReactionTimeGame::cleanup() {
  Timer1.detachInterrupt();
  Timer1.stop();
}

const char *ReactionTimeGame::getName() { return "Reaction Time"; }
