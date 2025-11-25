#pragma once
#include <Adafruit_Keypad.h>



class Keyboard
{
public:
    Keyboard();
    void update();

    struct KeyEvent
    {
        enum class Type : uint8_t 
        {
            PRESS,
            RELEASE
        };
        char key;
        Type type;
    };

    bool hasEvent() const;
    KeyEvent nextEvent();

private:
    Adafruit_Keypad keypad;

    static constexpr uint8_t MAX_EVENTS = 16;
    KeyEvent buffer[MAX_EVENTS];
    uint8_t head = 0;
    uint8_t tail = 0;
    uint32_t lastKeyPressTime = 0;
};
