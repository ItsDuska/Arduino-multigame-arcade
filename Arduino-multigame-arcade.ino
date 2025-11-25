

//#define NO_BOARD


//#include "C:\Users\ollis\Documents\Arduino\libraries\GFX_Library_for_Arduino\src\Arduino_GFX_Library.h"


#include "src/GameManager.h"

GameManager gameManager;

void setup()
{
  Serial.begin(9600);
  gameManager.init();
}

void loop() {
  gameManager.update();
}
