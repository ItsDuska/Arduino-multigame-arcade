#include <minigames/ButtonMash.h>

ButtonMashGame::ButtonMashGame()
    : score(0), startTime(0), gameComplete(false), lastButtonState(false) {}

void ButtonMashGame::init(Arduino_GFX &gfx) {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  score = 0;
  startTime = millis();
  gameComplete = false;
  lastButtonState = digitalRead(BUTTON_PIN);

  Serial.println("=== BUTTON MASH ===");
  Serial.println("Press button as fast as you can!");
}

void ButtonMashGame::update(uint32_t deltaTime, Keyboard &keyboard,
                            Joystick &Joystick) {
  if (gameComplete) {
    return;
  }

  if (millis() - startTime >= GAME_DURATION) {
    gameComplete = true;
    Serial.print("Final Score: ");
    Serial.println(score);
    return;
  }

  while (keyboard.hasEvent()) {
    Keyboard::KeyEvent keyEvent = keyboard.nextEvent();

    if (keyEvent.type == Keyboard::KeyEvent::Type::PRESS) {
      score++;
      Serial.print("Score: ");
      Serial.println(score);
    }
  }
}

void ButtonMashGame::render(uint32_t deltaTime, Arduino_GFX &gfx) {
  gfx.fillScreen(RGB565_BLACK);
  gfx.setCursor(random(gfx.width()), random(gfx.height()));
  gfx.setTextColor(random(0xffff), random(0xffff));
  gfx.setTextSize(random(6), random(6), random(2));
  gfx.println("mashaa nappeja");
}

void ButtonMashGame::cleanup() { pinMode(BUTTON_PIN, INPUT); }

const char *ButtonMashGame::getName() { return "Button Mash"; }
