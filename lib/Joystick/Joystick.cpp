#include "Joystick.h"
#include <Arduino.h>



// 0-100 degree range for joystick.
constexpr uint8_t JOYSTICK_BEGIN_VAL = 0;
constexpr uint8_t JOYSTICK_END_VAL = 100;
constexpr int8_t JOYSTICK_DEADZONE_VAL = 7;

Joystick::Joystick(u8 pinA, u8 pinB, u8 pinC)
{
    this->pinA = pinA;
    this->pinB = pinB;
    this->pinC = pinC;

    pinMode(pinA, INPUT);      
    pinMode(pinB, INPUT);        

    pinMode(pinC, INPUT_PULLUP);
}

Vec2u16 Joystick::readPosition()
{
    position.x = map(analogRead(pinA), 0, 1023, -100, 100);
    position.y = map(analogRead(pinB), 0, 1023, -100, 100);


    if (abs(position.x) < JOYSTICK_DEADZONE_VAL) 
    {
        position.x = 0;
    }
    else if (abs(position.y) < JOYSTICK_DEADZONE_VAL)
    {
        position.y = 0;
    }
    return position;
}

bool Joystick::isJoystickDown()
{
    buttonDown = !(bool)analogRead(pinC);
    return buttonDown;
}

Joystick::Direction Joystick::convertPositionToDirection()
{
    int16_t dx = position.x; 
    int16_t dy = position.y;

    if (abs(dx) < JOYSTICK_DEADZONE_VAL) 
    {
        dx = 0;
    }
    if (abs(dy) < JOYSTICK_DEADZONE_VAL)
    {
        dy = 0;
    }

    bool right = dx > 15;
    bool left  = dx < -15;
    bool up    = dy < -15;
    bool down  = dy > 15;

    if (up && right) return Direction::UP_RIGHT;
    if (up && left)  return Direction::LEFT_UP;
    if (down && right) return Direction::RIGHT_DOWN;
    if (down && left)  return Direction::DOWN_LEFT;

    if (up) return Direction::UP;
    if (down) return Direction::DOWN;
    if (right) return Direction::RIGHT;
    if (left) return Direction::LEFT;

    return Direction::IDLE;
}
