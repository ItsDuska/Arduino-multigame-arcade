#pragma once
#include <Arduino.h>

struct Vec2u16
{
    uint16_t x;
    uint16_t y;
};

// Position 50, 50 is 0,0
class Joystick
{
public:
    Joystick(uint8_t pinA, uint8_t pinB, uint8_t pinC);

    // reads the position and returns it. Also saves it internally
    Vec2u16 readPosition(void);
    bool isJoystickDown();

    // util functions
    enum class Direction
    {
        UP,
        UP_RIGHT,
        RIGHT,
        RIGHT_DOWN,
        DOWN,
        DOWN_LEFT,
        LEFT,
        LEFT_UP,
        IDLE
    };

    Direction convertPositionToDirection();

private:
    uint8_t pinA;
    uint8_t pinB;
    uint8_t pinC;

    bool buttonDown;
    Vec2u16 position;
};
