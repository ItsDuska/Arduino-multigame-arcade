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

//GameManager gameManager;
PlayerStatManager playerStatManager(0);


void setup() {
  Serial.begin(9600);

  //playerStatManager.add(15);
  //playerStatManager.add(18);
  //playerStatManager.add(1);

  //gameManager.init();
}

void loop() { //gameManager.update();

    PlayerStat* stats = playerStatManager.getStats();
    for (int i = 0; i < MAX_PLAYER_COUNT; i++) {
        Serial.println(stats[i].score);
    }
    playerStatManager.save();
    Serial.println("saved");
    delay(1000);

}

// --- TÄMÄ OSA KÄÄNNETÄÄN VAIN PC:LLÄ ---
#ifdef TARGET_PC

// Alustetaan staattiset muuttujat
int MockInputState::joyX = 512;
int MockInputState::joyY = 512;
bool MockInputState::joyBtn = false;
char MockInputState::lastKeyPressed = 0;
bool MockInputState::newKeyAvailable = false;

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL Init failed: %s\n", SDL_GetError());
    return 1;
  }

  setup();

  bool quit = false;
  SDL_Event e;
  const Uint8 *kbState = SDL_GetKeyboardState(NULL);

  while (!quit) {
    uint32_t startTicks = SDL_GetTicks();

    // --- INPUTTIEN LUKU (SDL -> MockInputState) ---
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        quit = true;

      // Keypad simulaatio
      if (e.type == SDL_KEYDOWN) {
        char mappedKey = 0;
        switch (e.key.keysym.sym) {
        // --- RIVI 1 ---
        case SDLK_1:
          mappedKey = '1';
          break;
        case SDLK_2:
          mappedKey = '2';
          break;
        case SDLK_3:
          mappedKey = '3';
          break;
        case SDLK_4:
          mappedKey = 'A';
          break;

        // --- RIVI 2 ---
        case SDLK_q:
          mappedKey = '4';
          break;
        case SDLK_w:
          mappedKey = '5';
          break;
        case SDLK_e:
          mappedKey = '6';
          break;
        case SDLK_r:
          mappedKey = 'B';
          break;

        // --- RIVI 3 ---
        case SDLK_a:
          mappedKey = '7';
          break;
        case SDLK_s:
          mappedKey = '8';
          break;
        case SDLK_d:
          mappedKey = '9';
          break;
        case SDLK_f:
          mappedKey = 'C';
          break;

        // --- RIVI 4 ---
        case SDLK_z:
          mappedKey = '*';
          break;
        case SDLK_x:
          mappedKey = '0';
          break;
        case SDLK_c:
          mappedKey = '#';
          break;
        case SDLK_v:
          mappedKey = 'D';
          break;

        // --- NUMPAD VAIHTOEHDOT (Jos haluat käyttää numpadia) ---
        case SDLK_KP_1:
          mappedKey = '1';
          break;
        case SDLK_KP_ENTER:
          mappedKey = '#';
          break;

        default:
          break; // Muut napit jätetään huomiotta
        }

        // Jos nappi löytyi listasta, välitä se eteenpäin
        if (mappedKey != 0) {
          MockInputState::lastKeyPressed = mappedKey;
          MockInputState::newKeyAvailable = true;

          // Debuggaus: Näet konsolissa mitä painettiin
          printf("PC Key: %d -> Mapped to: %c\n", e.key.keysym.sym, mappedKey);
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
