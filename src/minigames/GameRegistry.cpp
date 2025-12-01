#include <minigames/GameRegistry.h>

//#include "minigames/ButtonMash.h"
#include "minigames/FallingBlocks.h"
#include "minigames/Maze.h"
//#include "minigames/ReactionTime.h"
//#include "minigames/SpinJoyStickGame.h"

Game *createGame(uint8_t gameIndex) {
  switch (gameIndex) {
  // INFO: Jokaisella pelillä on oma index, se index arvotaan aina
  // satunnaisesti
  case 0:
    return new FallingBlocks();
  //case 1:
    //return new Maze();
    // return new ReactionTimeGame();
    // case 2: return new JoystickSpinGame();
    // case 3: return new FallingBlocks();

  default:
    return nullptr;
  }
}
