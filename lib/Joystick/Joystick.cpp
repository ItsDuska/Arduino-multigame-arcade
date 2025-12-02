#include "Joystick.h"

#ifdef TARGET_PC
#include "MockInputState.h"
#else
#include <Arduino.h>
#endif

// -100-100 degree range for joystick.
constexpr int8_t JOYSTICK_BEGIN_VAL = -100;
constexpr int8_t JOYSTICK_END_VAL = 100;
constexpr int8_t JOYSTICK_DEADZONE_VAL = 7;

Joystick::Joystick(uint8_t pinA, uint8_t pinB, uint8_t pinC) {
  this->pinA = pinA;
  this->pinB = pinB;
  this->pinC = pinC;

  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);

  pinMode(pinC, INPUT_PULLUP);
}

void Joystick::update() {
  int rawX, rawY;

#ifdef TARGET_PC
  rawX = MockInputState::joyX;
  rawY = MockInputState::joyY;
#else
  rawX = analogRead(pinA);
  rawY = analogRead(pinB);
#endif

  position.x = map(rawX, 0, 1023, JOYSTICK_BEGIN_VAL, JOYSTICK_END_VAL);
  position.y = map(rawY, 0, 1023, JOYSTICK_BEGIN_VAL, JOYSTICK_END_VAL);

  if (abs(position.x) < JOYSTICK_DEADZONE_VAL) {
    position.x = 0;
  } else if (abs(position.y) < JOYSTICK_DEADZONE_VAL) {
    position.y = 0;
  }
}

Vec2i16 Joystick::getPosition() { return position; }

bool Joystick::isJoystickDown() {
#ifdef TARGET_PC
  buttonDown = MockInputState::joyBtn;
#else
  buttonDown = !(bool)digitalRead(pinC);
#endif
  return buttonDown;
}

Joystick::Direction Joystick::convertPositionToDirection() {
  int16_t dx = position.x;
  int16_t dy = position.y;

  if (abs(dx) < JOYSTICK_DEADZONE_VAL) {
    dx = 0;
  }
  if (abs(dy) < JOYSTICK_DEADZONE_VAL) {
    dy = 0;
  }

  bool right = dx > 15;
  bool left = dx < -15;
  bool up = dy < -15;
  bool down = dy > 15;

  if (up && right)
    return Direction::UP_RIGHT;
  if (up && left)
    return Direction::LEFT_UP;
  if (down && right)
    return Direction::RIGHT_DOWN;
  if (down && left)
    return Direction::DOWN_LEFT;

  if (up)
    return Direction::UP;
  if (down)
    return Direction::DOWN;
  if (right)
    return Direction::RIGHT;
  if (left)
    return Direction::LEFT;

  return Direction::IDLE;
}
