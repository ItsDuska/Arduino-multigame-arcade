#include "MockDisplay.h"
#include "glcdfont.h"
#include <cmath>

// Staattisten alustus
SDL_Window *Arduino_GFX::window = nullptr;
SDL_Renderer *Arduino_GFX::renderer = nullptr;
SDL_Texture *Arduino_GFX::texture = nullptr;
Arduino_GFX *Arduino_GFX::instance = nullptr;

Arduino_GFX::Arduino_GFX(int w, int h) : _width(w), _height(h) {
  instance = this;

  // Alusta SDL vain kerran
  if (!window) {
    SDL_Init(SDL_INIT_VIDEO);
    // Luodaan ikkuna
    window = SDL_CreateWindow("Embedded Sim", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, w * 2, h * 2,
                              SDL_WINDOW_SHOWN); // 2x Zoom

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // Luodaan tekstuuri oikealla resoluutiolla (ei zoomattu)
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING, w, h);
  }

  pixels = new uint32_t[w * h];
  fillScreen(0); // Tyhjennä mustaksi aluksi
}

void Arduino_GFX::refreshGlobalInstance() {
  if (instance)
    instance->renderSDL();
}

void Arduino_GFX::renderSDL() {
  if (!renderer || !texture)
    return;
  SDL_UpdateTexture(texture, NULL, pixels, _width * sizeof(uint32_t));
  SDL_RenderCopy(renderer, texture, NULL,
                 NULL); // SDL skaalaa automaattisesti ikkunan kokoon
  SDL_RenderPresent(renderer);
}

// --- VÄRIMUUNNOS (565 -> 8888) ---
uint32_t Arduino_GFX::color565to8888(uint16_t color) {
  uint8_t r = (color >> 11) << 3;
  uint8_t g = ((color >> 5) & 0x3F) << 2;
  uint8_t b = (color & 0x1F) << 3;
  // Täysi alpha (FF)
  return (0xFF000000 | (r << 16) | (g << 8) | b);
}

// --- PERUSPIIRTO ---
void Arduino_GFX::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if (x < 0 || x >= _width || y < 0 || y >= _height)
    return;
  pixels[y * _width + x] = color565to8888(color);
}

void Arduino_GFX::fillScreen(uint16_t color) {
  uint32_t c = color565to8888(color);
  for (int i = 0; i < _width * _height; i++) {
    pixels[i] = c;
  }
}

// Tämä on tärkein funktio pelien kannalta (palkit, taustat)
void Arduino_GFX::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                           uint16_t color) {
  // Leikkaus (Clipping) - estetään ylivuodot
  if (x >= _width || y >= _height)
    return;
  if (x + w > _width)
    w = _width - x;
  if (y + h > _height)
    h = _height - y;
  if (x < 0) {
    w += x;
    x = 0;
  }
  if (y < 0) {
    h += y;
    y = 0;
  }

  uint32_t c = color565to8888(color);
  for (int j = y; j < y + h; j++) {
    for (int i = x; i < x + w; i++) {
      pixels[j * _width + i] = c;
    }
  }
}

void Arduino_GFX::drawRect(int16_t x, int16_t y, int16_t w, int16_t h,
                           uint16_t color) {
  // Piirretään 4 viivaa käyttäen fillRectiä (1px paksuus)
  fillRect(x, y, w, 1, color);         // Top
  fillRect(x, y + h - 1, w, 1, color); // Bottom
  fillRect(x, y, 1, h, color);         // Left
  fillRect(x + w - 1, y, 1, h, color); // Right
}

void Arduino_GFX::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                           uint16_t color) {
  // Bresenhamin algoritmi (Yksinkertaistettu)
  int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2;

  while (true) {
    drawPixel(x0, y0, color);
    if (x0 == x1 && y0 == y1)
      break;
    e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx) {
      err += dx;
      y0 += sy;
    }
  }
}

void Arduino_GFX::drawCircle(int16_t x0, int16_t y0, int16_t r,
                             uint16_t color) {
  // Midpoint circle algorithm
  int x = r, y = 0;
  int err = 0;
  while (x >= y) {
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 - y, y0 - x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 + x, y0 - y, color);
    if (err <= 0) {
      y += 1;
      err += 2 * y + 1;
    }
    if (err > 0) {
      x -= 1;
      err -= 2 * x + 1;
    }
  }
}

// --- TEKSTIN KÄSITTELY ---
void Arduino_GFX::setCursor(int16_t x, int16_t y) {
  cursor_x = x;
  cursor_y = y;
}

void Arduino_GFX::setTextColor(uint16_t c) { textcolor = c; }
void Arduino_GFX::setTextColor(uint16_t c, uint16_t bg) {
  textcolor = c;
  textbgcolor = bg;
}
void Arduino_GFX::setTextSize(uint8_t s) {
  textsize_x = s;
  textsize_y = s;
}
void Arduino_GFX::setTextSize(uint8_t sx, uint8_t sy, uint8_t pixel_margin) {
  textsize_x = sx;
  textsize_y = sy;
}

void Arduino_GFX::drawChar(int16_t x, int16_t y, unsigned char c,
                           uint16_t color, uint16_t bg, uint8_t size_x,
                           uint8_t size_y) {
  if ((x >= _width) || (y >= _height) || ((x + 6 * size_x - 1) < 0) ||
      ((y + 8 * size_y - 1) < 0))
    return;

  for (int8_t i = 0; i < 5; i++) { // Fontti on 5 pikseliä leveä
    uint8_t line = font[(c * 5) + i];
    for (int8_t j = 0; j < 8; j++, line >>= 1) { // 8 pikseliä korkea
      if (line & 1) {
        if (size_x == 1 && size_y == 1)
          drawPixel(x + i, y + j, color);
        else
          fillRect(x + i * size_x, y + j * size_y, size_x, size_y, color);
      } else if (bg != color) {
        if (size_x == 1 && size_y == 1)
          drawPixel(x + i, y + j, bg);
        else
          fillRect(x + i * size_x, y + j * size_y, size_x, size_y, bg);
      }
    }
  }
  // Oikean reunan väli (6. pikselisarake)
  if (bg != color) {
    if (size_x == 1 && size_y == 1)
      for (int8_t j = 0; j < 8; j++)
        drawPixel(x + 5, y + j, bg);
    else
      fillRect(x + 5 * size_x, y, size_x, 8 * size_y, bg);
  }
}

void Arduino_GFX::print(const char *text) {
  while (*text) {
    if (*text == '\n') {
      cursor_y += textsize_y * 8;
      cursor_x = 0;
    } else if (*text == '\r') {
      // skip
    } else {
      if (cursor_x + textsize_x * 6 >= _width) { // Rivinvaihto jos menee yli
        cursor_x = 0;
        cursor_y += textsize_y * 8;
      }

      drawChar(cursor_x, cursor_y, *text, textcolor, textbgcolor, textsize_x,
               textsize_y);
      cursor_x += textsize_x * 6;
    }
    text++;
  }
}

void Arduino_GFX::println(const char *text) {
  print(text);
  cursor_x = 0;                   // Oikeasti pitäisi palata marginaaliin
  cursor_y += 8 * textsize_y + 2; // Rivinvaihto
}

void Arduino_GFX::print(int n) { print(std::to_string(n).c_str()); }
void Arduino_GFX::print(double n) { print(std::to_string(n).c_str()); }
void Arduino_GFX::println(int n) { println(std::to_string(n).c_str()); }
void Arduino_GFX::println(double n) { println(std::to_string(n).c_str()); }
