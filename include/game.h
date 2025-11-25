#pragma once

/* - NOTES:
Jokaisen pelin pitää sulkeutuessa palauttaa score ja bool
 arvo siitä että kuoltiinko vai ei.
----------------------

interupt ajastin pelin jokaiselle minipelille eli sulla on vaan n määrä aikaa, sitten peli loppuu

 */
/*
constexpr u8 MAX_USER_NAME_LENGHT = 4;
constexpr u8 MAX_USER_AMOUNT = 5; // tallenna aina top n useria.

// Tää tallennetaan aina lopussa eepromiin.
struct User
{
    u8 name[MAX_USER_NAME_LENGHT];
    u16 score;
};


enum class State : u8
{
    MENU,
    GAME,
    LOAD_GAME,
    END_GAME
};


// peli staten sisällä on bool hasWon state. Katsomme vasta update functiossa sen
// onko peli voitettu vai ei, koska interupt voi tapahtua milloin vain.


// näiden pitää olla func pointereita ja sitten tallennamme listna minipelejä tähän.



constexpr u8 MAX_LOSE_STREAK = 3;

struct Score
{
    u16 currentScore;
    u16 winStreak;
    u16 loseStreak;
};

// GLOBALS
State currentState = State::MENU;
Score score{};
MiniGame currentMinigame;

void setupMinigame(u16 maxTime, u16 miniGameOffset)
{
    currentMinigame.init();
    //setupTimer(); se interupti siis tähän.
}

void switchMiniGame(bool lastGameSatus)
{
    if (lastGameSatus)
    {
        score.winStreak++;
    }
    score.currentScore += currentMinigame.clean(); // add new score;
    // generate random number to select the next minigame.
    // u16 nextGameIndex = generateRandomMiniGameIndex();

    currentState = State::LOAD_GAME;
}


// Tapahtuu kun n sekunttia on käyny.
// Aika loppui joten pelaaja häviää.
void exampleInterupt()
{
    currentMinigame.stop(false);
    score.loseStreak += 1;
}


// main func example
void loop()
{
    if (!currentMinigame.update())
    {
        switchMiniGame(false);
    }

    currentMinigame.render(nullptr);

    if (currentMinigame.isGameWon())
    {
        switchMiniGame(true);
    }
}
*/
