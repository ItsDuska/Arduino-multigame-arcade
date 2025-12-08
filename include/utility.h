#pragma once

#include "HardwareWrapper.h"

inline void utilityPrintCenter(Arduino_GFX &gfx, const char *text, uint8_t textSize, int16_t offsetX = 0, int16_t offsetY = 0) {
  const int charW = 6 * textSize;
  const int charH = 8 * textSize;

  const int textW = strlen(text) * charW;
  const int textH = charH;

  const int x = (gfx.width() - textW) / 2 + offsetX;
  const int y = (gfx.height() - textH) / 2 + offsetY;

  gfx.setTextSize(textSize);
  gfx.setCursor(x, y); // top-left of the classic font bounding box
  gfx.print(text);
}
