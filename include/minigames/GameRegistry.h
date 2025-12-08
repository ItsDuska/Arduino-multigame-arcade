#ifndef GAME_REGISTRY_H
#define GAME_REGISTRY_H

#include "GameInterface.h"

Game *createGame(uint8_t gameIndex);

constexpr uint8_t GAME_COUNT = 4;

#endif
