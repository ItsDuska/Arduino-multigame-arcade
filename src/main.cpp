#include <Arduino.h>
#include <Joystick.h>

Joystick joystick(A0, A1, A2);

void setup() { Serial.begin(9600); }

void loop() {
  joystick.readPosition();
  joystick.getPosition();
  switch (joystick.convertPositionToDirection()) {
  case Joystick::Direction::UP:
    Serial.println("UP");
    break;
  case Joystick::Direction::UP_RIGHT:
    Serial.println("UP_RIGHT");
    break;
  case Joystick::Direction::RIGHT:
    Serial.println("RIGHT");
    break;
  case Joystick::Direction::RIGHT_DOWN:
    Serial.println("RIGHT_DOWN");
    break;
  case Joystick::Direction::DOWN:
    Serial.println("DOWN");
    break;
  case Joystick::Direction::DOWN_LEFT:
    Serial.println("DOWN_LEFT");
    break;
  case Joystick::Direction::LEFT:
    Serial.println("LEFT");
    break;
  case Joystick::Direction::LEFT_UP:
    Serial.println("LEFT_UP");
    break;
  case Joystick::Direction::IDLE:
    Serial.println("IDLE");
    break;
  }
}
