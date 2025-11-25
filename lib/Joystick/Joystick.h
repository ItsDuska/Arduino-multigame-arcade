#pragma once
#include <Arduino.h>


// TODO: muuta tän nimi vec2i16 kun saadaan joystick toimimaan.
struct Vec2u16
{
    int16_t x;
    int16_t y;
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
