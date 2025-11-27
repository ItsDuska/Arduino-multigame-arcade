#include "GameManager.h"

// --- PC SIMULAATION VAATIMAT OSAT ---
#ifdef TARGET_PC
#include "MockDisplay.h"
#include "MockInputState.h"
#include <SDL2/SDL.h>

// Globaali viittaus näyttöön, jotta main-loop voi päivittää ikkunan
extern SDL_Window *sdlWindow;
extern SDL_Renderer *sdlRenderer;
#endif

GameManager gameManager;

void setup() {
  Serial.begin(9600);
  gameManager.init();
}

void loop() { gameManager.update(); }

// --- TÄMÄ OSA KÄÄNNETÄÄN VAIN PC:LLÄ ---
#ifdef TARGET_PC

// Alustetaan staattiset muuttujat
int MockInputState::joyX = 512;
int MockInputState::joyY = 512;
bool MockInputState::joyBtn = false;
char MockInputState::lastKeyPressed = 0;
bool MockInputState::newKeyAvailable = false;

int main(int argc, char *argv[]) {
  // 1. Alusta SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL Init failed: %s\n", SDL_GetError());
    return 1;
  }

  // 2. Kutsu Arduinon setup()
  setup();

  bool quit = false;
  SDL_Event e;
  const Uint8 *kbState = SDL_GetKeyboardState(NULL);

  // 3. Simulaattorin "Hardware Loop"
  while (!quit) {
    uint32_t startTicks = SDL_GetTicks();

    // --- INPUTTIEN LUKU (SDL -> MockInputState) ---
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        quit = true;

      // Keypad simulaatio (NumPad tai numerot)
      if (e.type == SDL_KEYDOWN) {
        char key = 0;
        if (e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) {
          key = '0' + (e.key.keysym.sym - SDLK_0);
        } else if (e.key.keysym.sym >= SDLK_a && e.key.keysym.sym <= SDLK_d) {
          key = 'A' + (e.key.keysym.sym - SDLK_a);
        }

        if (key != 0) {
          MockInputState::lastKeyPressed = key;
          MockInputState::newKeyAvailable = true;
        }
      }
    }

    // Joystick simulaatio (Nuolinäppäimet -> Analog arvot)
    MockInputState::joyX = 512;
    MockInputState::joyY = 512;
    if (kbState[SDL_SCANCODE_UP])
      MockInputState::joyY = 1023; // Tai 0 riippuen kytkennästä
    if (kbState[SDL_SCANCODE_DOWN])
      MockInputState::joyY = 0;
    if (kbState[SDL_SCANCODE_LEFT])
      MockInputState::joyX = 0;
    if (kbState[SDL_SCANCODE_RIGHT])
      MockInputState::joyX = 1023;
    MockInputState::joyBtn = kbState[SDL_SCANCODE_RETURN]; // Enter = nappi

    // --- ARDUINO LOOP ---
    loop();
    Arduino_GFX::refreshGlobalInstance();

    uint32_t frameTime = SDL_GetTicks() - startTicks;
    if (frameTime < 16)
      SDL_Delay(16 - frameTime);
  }

  SDL_Quit();
  return 0;
}
#endif
