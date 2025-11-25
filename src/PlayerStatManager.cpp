#include "PlayerStatManager.h"
#include <string.h>
#include <EEPROM.h>

PlayerStatManager::PlayerStatManager(uint16_t eepromAddress)
    : baseAddress(eepromAddress)
{
}

void PlayerStatManager::add(const PlayerStat &newStat)
{
    // Näiden pitäisi olla valmiiksi jo järjestyksessä,
    //  joten tämän voisi vetää nopeammin, mutta se ei niin haittaa,
    //  sillä tätä kutsutaan niin harvoin.
    if (uniquePlayer(newStat))
    {
        return;
    }

    stats[MAX_PLAYER_COUNT - 1] = newStat;

    for (int i = 0; i < MAX_PLAYER_COUNT - 1; i++)
    {
        for (int j = i + 1; j < MAX_PLAYER_COUNT; j++)
        {
            if (stats[j].score > stats[i].score)
            {
                PlayerStat temp = stats[i];
                stats[i] = stats[j];
                stats[j] = temp;
            }
        }
    }
}

void PlayerStatManager::save()
{
    int addr = baseAddress;
    for (int i = 0; i < MAX_PLAYER_COUNT; i++)
    {

        for (int j = 0; j < 5; j++)
        {
            EEPROM.update(addr++, stats[i].name[j]);
        }

        for (int j = 0; j < 2; j++)
        {
            EEPROM.update(addr++, (stats[i].score >> (8 * j)) & 0xFF);
        }
    }
}

void PlayerStatManager::read()
{
    uint16_t addr = baseAddress;
    for (int i = 0; i < MAX_PLAYER_COUNT; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            stats[i].name[j] = EEPROM.read(addr++);
        }

        stats[i].score = 0;
        for (int j = 0; j < 2; j++)
        {
            stats[i].score |= ((uint16_t)EEPROM.read(addr++)) << (8 * j);
        }
    }
}

bool PlayerStatManager::uniquePlayer(const PlayerStat &newStat)
{
    for (uint8_t i = 0; i < MAX_PLAYER_COUNT; i++)
    {
        if (strncmp(stats[i].name, newStat.name, MAX_NAME_LEN))
        {
            if (newStat.score > stats[i].score)
            {
                stats[i].score = newStat.score;
            }
            return false;
        }
    }
    return true;
}