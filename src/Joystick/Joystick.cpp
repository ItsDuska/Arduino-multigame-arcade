#include "Joystick.h"
#include <Arduino.h>


// 0-100 degree range for joystick.
constexpr uint8_t JOYSTICK_BEGIN_VAL = 0;
constexpr uint8_t JOYSTICK_END_VAL = 100;


Joystick::Joystick(u8 pinA, u8 pinB, u8 pinC)
{
    this->pinA = pinA;
    this->pinB = pinB;
    this->pinC = pinC;

    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
    pinMode(pinC, INPUT);

    digitalWrite(pinC, HIGH);
}

Vec2u16 Joystick::readPosition()
{
    position.x = map(analogRead(this->pinA), 0, 1023, JOYSTICK_BEGIN_VAL, JOYSTICK_END_VAL);
    position.y = map(analogRead(this->pinB), 0, 1023, JOYSTICK_BEGIN_VAL, JOYSTICK_END_VAL);
    return position;
}

bool Joystick::isJoystickDown()
{
    buttonDown = !(bool)analogRead(pinC);
    return buttonDown;
}

// Convert 0-100 xy coords to wind direction enum.
Joystick::Direction Joystick::convertPositionToDirection()
{
    // 50,50 == middle
    int16_t dx = position.x - 50;
    int16_t dy = position.y - 50;

    if (dx > -5 && dx < 5 && dy > -5 && dy < 5)
    {
        return Direction::IDLE;
    }

    uint8_t right = (dx > 15);
    uint8_t left = (dx < -15);
    uint8_t up = (dy < -15);
    uint8_t down = (dy > 15);

    if (up && right) return Direction::UP_RIGHT;
    if (up && left) return Direction::LEFT_UP;
    if (down && right) return Direction::RIGHT_DOWN;
    if (down && left) return Direction::DOWN_LEFT;

    if (up) return Direction::UP;
    if (down) return Direction::DOWN;
    if (right) return Direction::RIGHT;
    if (left) return Direction::LEFT;

    return Direction::IDLE;
}
