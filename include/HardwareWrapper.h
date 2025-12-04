#pragma once

#ifdef TARGET_PC
// --- SIMULAATION KIRJASTOT ---
#include <MockArduino.h>        // Delay, millis, Serial
#include <MockDisplay.h>        // Korvaa Arduino_GFX
#include <MockDisplayDrivers.h> // Korvaa Arduino_DataBus ja Arduino_R61529
#include <MockEEPROM.h>         // Korvaa EEPROM
#include <MockInputState.h>     // Simuloi joystickin ja näppäimistön tilaa
#include <MockKeypad.h>         // Korvaa Adafruit_Keypad
#include <MockTimerOne.h>       // Korvaa TimerOne
#include <SDL2/SDL.h>

#ifndef RGB565_BLACK
#define RGB565_BLACK 0x0000
#define RGB565_BLUE 0x001F
#define RGB565_RED 0xF800
#define RGB565_GREEN 0x07E0
#define RGB565_CYAN 0x07FF
#define RGB565_MAGENTA 0xF81F
#define RGB565_YELLOW 0xFFE0
#define RGB565_WHITE 0xFFFF
#endif

#else
// --- OIKEAT KIRJASTOT ---
#include "Joystick.h"
#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <EEPROM.h>
#include <TimerOne.h>
#endif
