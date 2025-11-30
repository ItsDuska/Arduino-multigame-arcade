#include <minigames/SpinJoyStickGame.h>

JoystickSpinGame::JoystickSpinGame()
    : startTime(0), score(0), gameComplete(false), currentStage(Stage::UP) {}

void JoystickSpinGame::init(Arduino_GFX &gfx) {
  startTime = millis();
  score = 0;
  currentStage = Stage::UP;
  gameComplete = false;

  Serial.println("=== JOYSTICK SPIN ===");
  Serial.println("Rotate the joystick clockwise as fast as you can!");
  Serial.println("UP → RIGHT → DOWN → LEFT → UP");
}

void JoystickSpinGame::update(uint32_t deltaTime, Keyboard &Keyboard,
                              Joystick &joystick) {
  if (gameComplete)
    return;

  if (millis() - startTime >= GAME_DURATION) {
    gameComplete = true;
    Serial.print("Final Score: ");
    Serial.println(score);
    return;
  }

  joystick.update();
  auto dir = joystick.convertPositionToDirection();

  switch (currentStage) {
  case Stage::UP:
    if (dir == Joystick::Direction::UP)
      currentStage = Stage::RIGHT;
    break;

  case Stage::RIGHT:
    if (dir == Joystick::Direction::RIGHT)
      currentStage = Stage::DOWN;
    break;

  case Stage::DOWN:
    if (dir == Joystick::Direction::DOWN)
      currentStage = Stage::LEFT;
    break;

  case Stage::LEFT:
    if (dir == Joystick::Direction::LEFT) {
      score++;
      Serial.print("Score: ");
      Serial.println(score);
      currentStage = Stage::UP;
    }
    break;
  }
}

void JoystickSpinGame::render(uint32_t deltaTime, Arduino_GFX &gfx) {}

void JoystickSpinGame::cleanup() {
  // Nothing necessary here
}

const char *JoystickSpinGame::getName() { return "Joystick Spin"; }
