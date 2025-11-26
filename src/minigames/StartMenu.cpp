#include "minigames/StartMenu.h"

/*
constexpr char *menuStrings = {
    "Play",
    "Level Selector",
    "Stats",
    "Quit"};
*/

StartMenu::StartMenu(char *playerName) {}

void StartMenu::init() {}

void StartMenu::update(uint32_t deltaTime, Keyboard &keyboard,
                       Joystick &Joystick) {}

void StartMenu::render(uint32_t deltaTime, Arduino_GFX &gfx) {}

void StartMenu::cleanup() {}

bool StartMenu::isComplete() { return false; }

const char *StartMenu::getName() { return nullptr; }
