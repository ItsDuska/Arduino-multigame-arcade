#pragma once
#include "GameInterface.h"

class JoystickSpinGame : public Game
{
public:
    JoystickSpinGame();

    void init();
    void update(uint32_t deltaTime, Keyboard& keyboard, Joystick& joystick) override;
    void render(uint32_t deltaTime, Arduino_GFX& gfx) override;
    void cleanup();

    bool isComplete();
    const char* getName();

private:
    enum class Stage : uint8_t
    {
        UP,
        RIGHT,
        DOWN,
        LEFT
    };

    uint32_t startTime;
    int score;
    bool gameComplete;

    Stage currentStage;
    static constexpr uint32_t GAME_DURATION = 10000; // 10s time
};
