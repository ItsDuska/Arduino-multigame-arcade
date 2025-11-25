#pragma once
#include <stdint.h>

constexpr uint8_t MAX_PLAYER_COUNT = 5;
constexpr uint8_t MAX_NAME_LEN = 5;

struct PlayerStat
{
    char name[5];
    uint16_t score;
};

class PlayerStatManager
{
public:
    PlayerStatManager(uint16_t eepromAddress = 0);
    void add(const PlayerStat &newStat);
    void save();
    void read();

private:
    bool uniquePlayer(const PlayerStat &newStat);

private:
    PlayerStat stats[MAX_PLAYER_COUNT];
    uint16_t baseAddress;
};
