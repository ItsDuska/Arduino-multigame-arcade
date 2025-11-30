#pragma once
#include "HardwareWrapper.h"

static constexpr uint8_t MAX_PLAYER_COUNT = 5;

struct PlayerStat {
  uint16_t score;
};

class PlayerStatManager {
public:
  PlayerStatManager(uint16_t eepromAddress);

  void add(uint16_t newScore);
  void save();
  void read();
  PlayerStat *getStats();

private:
  PlayerStat stats[MAX_PLAYER_COUNT];
  uint16_t baseAddress;
};
