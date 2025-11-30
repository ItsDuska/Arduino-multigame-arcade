#include <minigames/ReactionTime.h>

ReactionTimeGame::ReactionTimeGame()
    : phase(WAITING), phaseStartTime(0), waitDuration(0), reactionTime(0),
      gameComplete(false) {}

void ReactionTimeGame::init(Arduino_GFX &gfx) {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  phase = WAITING;
  phaseStartTime = millis();
  gameComplete = false;

  randomSeed(analogRead(A0));
  waitDuration = random(WAIT_TIME_MIN, WAIT_TIME_MAX);

  Serial.println("=== REACTION TIME (Keyboard) ===");
  Serial.print("Press ");
  Serial.print(REACT_KEY);
  Serial.println("when LED lights up!");
}

void ReactionTimeGame::update(uint32_t deltaTime, Keyboard &keyboard,
                              Joystick &joystick) {
  if (gameComplete) {
    return;
  }

  switch (phase) {
  case WAITING:
    if (millis() - phaseStartTime >= waitDuration) {
      digitalWrite(LED_PIN, HIGH);
      phase = LED_ON;
      phaseStartTime = millis();
    }
    break;

  case LED_ON:
    while (keyboard.hasEvent()) {
      Keyboard::KeyEvent ev = keyboard.nextEvent();
      if (ev.type == Keyboard::KeyEvent::Type::PRESS && ev.key == REACT_KEY) {
        reactionTime = millis() - phaseStartTime;
        Serial.print(F("Reaction time: "));
        Serial.print(reactionTime);
        Serial.println(F("ms"));

        phase = COMPLETE;
        gameComplete = true;
        break;
      }
    }
    break;

  case COMPLETE:
    break;
  }
}

void ReactionTimeGame::render(uint32_t deltaTime, Arduino_GFX &gfx) {}

void ReactionTimeGame::cleanup() {
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, INPUT);
}

const char *ReactionTimeGame::getName() { return "Reaction Time"; }
