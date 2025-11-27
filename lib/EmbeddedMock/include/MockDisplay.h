#pragma once
#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

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

class Arduino_GFX {
public:
  Arduino_GFX(int w, int h);
  ~Arduino_GFX() {} // Destruktori

  bool begin(int speed = 0) { return true; }

  // --- PRIMITIIVIT ---
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
  void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
  void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
                uint16_t bg, uint8_t size_x, uint8_t size_y);
  void fillScreen(uint16_t color);

  // --- TEKSTI (Simulaatio) ---
  void setCursor(int16_t x, int16_t y);
  void setTextColor(uint16_t c);
  void setTextColor(uint16_t c, uint16_t bg);
  void setTextSize(uint8_t s);
  void setTextSize(uint8_t sx, uint8_t sy,
                   uint8_t pixel_margin); // Lisätty ylikuormitus

  void print(const char *text);
  void print(int n);
  void print(double n);
  void println(const char *text);
  void println(int n);
  void println(double n);

  // --- MUUT ---
  void setRotation(uint8_t r) {
  } // Ei toteuteta kääntöä simulaattorissa (liian työlästä)
  int16_t width() { return _width; }
  int16_t height() { return _height; }

  // SDL Spesifinen
  void renderSDL();
  static void refreshGlobalInstance();

private:
  int _width, _height;

  // Tekstin tila
  int16_t cursor_x = 0;
  int16_t cursor_y = 0;
  uint16_t textcolor = 0xFFFF;
  uint16_t textbgcolor = 0x0000;
  uint8_t textsize_x = 1;
  uint8_t textsize_y = 1;

  // SDL jutut
  static SDL_Window *window;
  static SDL_Renderer *renderer;
  static SDL_Texture *texture;
  static Arduino_GFX *instance;
  uint32_t *pixels;

  uint32_t color565to8888(uint16_t color);
};
