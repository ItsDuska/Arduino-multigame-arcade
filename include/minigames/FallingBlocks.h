#pragma once
#include "GameInterface.h"


static constexpr uint8_t COLS = 5;
static constexpr uint8_t ROWS = 8;
static constexpr uint8_t MAX_OBSTACLES = 6;
static constexpr uint16_t SPAWN_INTERVAL = 700; // ms


struct Obstacle 
{
    int8_t x;     
    int8_t y;    
    bool active;
};


class FallingBlocks : public Game
{
public:
    FallingBlocks();
    void init() override;
    void update(uint32_t deltaTime, Keyboard& keyboard, Joystick& Joystick) override;
    void render(uint32_t deltaTime, Arduino_GFX& gfx) override;
    void cleanup() override;
    bool isComplete() override;
    const char* getName() override;    
private:
    void spawnObstacle(); 
    void moveObstacles();
    void checkCollision();
    void handlePlayerInput(Keyboard& keyboard, Joystick& joystick);


private:
    Obstacle obstacles[MAX_OBSTACLES];
    uint32_t lastSpawnTime;
    uint32_t lastMoveTime;
    uint16_t score;
    uint8_t playerX; 
    bool gameOver;
};


