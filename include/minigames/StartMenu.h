#pragma once
#include "GameInterface.h"

class StartMenu : public Game
{
public:
    StartMenu(char *playerName);
    void init() override;
    void update(uint32_t deltaTime, Keyboard &keyboard, Joystick &Joystick) override;
    void render(uint32_t deltaTime, Arduino_GFX &gfx) override;
    void cleanup() override;
    bool isComplete() override;
    const char *getName() override;

private:
};