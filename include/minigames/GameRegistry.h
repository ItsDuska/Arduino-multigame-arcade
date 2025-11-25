#ifndef GAME_REGISTRY_H
#define GAME_REGISTRY_H

#include "GameInterface.h"

#include "ButtonMash.h"
#include "ReactionTime.h"
#include "SpinJoyStickGame.h"
#include "FallingBlocks.h"



Game* createGame(uint8_t gameIndex);


constexpr uint8_t GAME_COUNT = 1;

#endif
