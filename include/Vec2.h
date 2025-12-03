#pragma once
#include "HardwareWrapper.h"

struct u16Vec2 {
  u16Vec2(uint16_t x, uint16_t y) : x(x), y(y) {}
  union {
    struct {
      uint16_t x;
      uint16_t y;
    };
    struct {
      uint16_t width;
      uint16_t height;
    };
  };
};

struct i16Vec2 {
  i16Vec2(int16_t x, int16_t y) : x(x), y(y) {}
  union {
    struct {
      int16_t x;
      int16_t y;
    };
    struct {
      int16_t width;
      int16_t height;
    };
  };
};

struct f16Vec2 {
  f16Vec2(float x, float y) : x(x), y(y) {}
  union {
    struct {
      float x;
      float y;
    };
    struct {
      float width;
      float height;
    };
  };
};

struct u8Vec2 {
  u8Vec2(uint8_t x, uint8_t y) : x(x), y(y) {}
  union {
    struct {
      uint8_t x;
      uint8_t y;
    };
    struct {
      uint8_t width;
      uint8_t height;
    };
  };
};

struct i8Vec2 {
  i8Vec2(int8_t x, int8_t y) : x(x), y(y) {}
  union {
    struct {
      int8_t x;
      int8_t y;
    };
    struct {
      int8_t width;
      int8_t height;
    };
  };
};
