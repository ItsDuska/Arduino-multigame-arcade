#include <minigames/ReactionTime.h>

// Global variables for TimerOne handling
static volatile bool timerFired = false;
static int countdownCounter = 3;
static bool isTimerRunning = false;

// Interrupt Service Routine
void reactionTimerISR() { timerFired = true; }

ReactionTimeGame::ReactionTimeGame()
    : phase(START), phaseStartTime(0), reactionTime(0), speedDisplayed(false) {}

void ReactionTimeGame::init(Arduino_GFX &gfx) {
  gfx.fillScreen(RGB565_BLACK);
  gfx.setTextColor(RGB565_WHITE);
  gfx.setTextSize(2);

  // Nollataan staattiset muuttujat
  timerFired = false;
  isTimerRunning = false;
  countdownCounter = 3;
  testCount = 0;

  gameComplete = false; // Varmistetaan että peli ei ole "valmis" heti alussa
};

void ReactionTimeGame::update(uint32_t deltaTime, Keyboard &keyboard,
                              Joystick &joystick) {
  if (gameComplete)
  {
    return;
  }
  switch (phase) {
  case START:
    if (!isTimerRunning) {
      Timer1.initialize(1000000); // 1 second
      Timer1.attachInterrupt(reactionTimerISR);
      isTimerRunning = true;
      timerFired = false;
    }

    if (timerFired) {
      timerFired = false;
      countdownCounter--;

      if (countdownCounter <= 0) {
        Timer1.detachInterrupt();
        isTimerRunning = false;
        phase = WAITING;
      }
    }
    break;

  case WAITING:
    if (!isTimerRunning) {
      uint32_t randomDelay =
          random(2000, 5000) * 1000; // Convert ms to microseconds
      Timer1.initialize(randomDelay);
      Timer1.attachInterrupt(reactionTimerISR);
      isTimerRunning = true;
      timerFired = false;
    }

    if (timerFired) {
      Timer1.detachInterrupt();
      isTimerRunning = false;
      phaseStartTime = millis();
      phase = REACTSCREEN;
    }
    break;

  case REACTSCREEN:
    while (keyboard.hasEvent()) {
      Keyboard::KeyEvent ev = keyboard.nextEvent();
      if (ev.type == Keyboard::KeyEvent::Type::PRESS) {
        reactionTime = millis() - phaseStartTime;
        Serial.println(phaseStartTime);
        phase = SPEED_DISPLAY;
        break;
      }
    }
    break;

  case SPEED_DISPLAY:
    while (keyboard.hasEvent()) {
      Keyboard::KeyEvent ev = keyboard.nextEvent();
      if (ev.type == Keyboard::KeyEvent::Type::PRESS) {
        testCount++;
        speedDisplayed = false; // Nollataan lippu seuraavaa kertaa varten

        if (testCount < TEST_ROUNDS) {
          phase = WAITING;
          isTimerRunning = false;
        } else {
          phase = COMPLETE;
        }
        break;
      }
    }
    break;

  case COMPLETE:
    Serial.print("Lopetus!");
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
  const int lineHeight = 30;
  const int countdownNumY = (titleY * 2 + lineHeight) + lineHeight;
  const int countdownNumX = screenWidth / 2;

  switch (phase) {
  case START:
    gfx.fillScreen(RGB565_BLACK);
    gfx.setCursor(titleX, titleY);
    gfx.print("Paina nappia kun");
    gfx.setCursor(titleX, titleY + 30);
    gfx.print("ruutu vaihtuu!");

    for (int i = 3; i > 0; i--) {
      // Pyyhitään edellinen numero
      gfx.fillRect(countdownNumX, countdownNumY, 40, 30, RGB565_BLACK);
      gfx.setCursor(countdownNumX, countdownNumY);
      gfx.print(i);
      delay(1000);
    }

    phase = WAITING;
    break;

  case WAITING:
    gfx.fillScreen(RGB565_BLACK);
    break;

  case REACTSCREEN:
    gfx.fillRect(150, 100, 10, 10, RGB565_CYAN);
    break;

  case SPEED_DISPLAY:
    if (speedDisplayed)
      break;

    gfx.fillScreen(RGB565_BLACK);
    gfx.setCursor(titleX, titleY + 50);
    gfx.print("Reaktioaika: ");
    gfx.print(reactionTime);
    gfx.print(" ms");
    speedDisplayed = true;
    break;

  case COMPLETE:
    break;
  }
}

void ReactionTimeGame::cleanup() { Timer1.detachInterrupt(); }

const char *ReactionTimeGame::getName() { return "Reaction Time"; }
