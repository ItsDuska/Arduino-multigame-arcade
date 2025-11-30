#include "PlayerStatManager.h"
#include <HardwareWrapper.h>

PlayerStatManager::PlayerStatManager(uint16_t eepromAddress)
    : baseAddress(eepromAddress) {
  read();
}

void PlayerStatManager::add(uint16_t newScore) {
  uint8_t minIndex = 0;
  for (uint8_t i = 1; i < MAX_PLAYER_COUNT; i++) {
    if (stats[i].score < stats[minIndex].score) {
      minIndex = i;
    }
  }

  if (newScore > stats[minIndex].score) {
    stats[minIndex].score = newScore;
  }

  for (uint8_t i = 0; i < MAX_PLAYER_COUNT - 1; i++) {
    for (uint8_t j = i + 1; j < MAX_PLAYER_COUNT; j++) {
      if (stats[j].score > stats[i].score) {
        uint16_t tmp = stats[i].score;
        stats[i].score = stats[j].score;
        stats[j].score = tmp;
      }
    }
  }
}

void PlayerStatManager::save() {
  uint16_t addr = baseAddress;
  for (uint8_t i = 0; i < MAX_PLAYER_COUNT; i++) {
    EEPROM.update(addr++, stats[i].score & 0xFF);
    EEPROM.update(addr++, (stats[i].score >> 8) & 0xFF);
  }
}

void PlayerStatManager::read() {
  uint16_t addr = baseAddress;
  for (uint8_t i = 0; i < MAX_PLAYER_COUNT; i++) {
    stats[i].score = EEPROM.read(addr++);
    stats[i].score |= ((uint16_t)EEPROM.read(addr++)) << 8;
  }
}

PlayerStat *PlayerStatManager::getStats() { return stats; }
