#pragma once

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <thread>

// --- VAKIOT ---
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define LOW 0
#define HIGH 1

// --- PINNIT (Arduino Mega mapattuna) ---
#define A0 54
#define A1 55
#define A2 56
#define A3 57
#define A4 58
#define A5 59
#define A6 60
#define A7 61
#define A8 62
#define A9 63
#define A10 64
#define A11 65
#define A12 66
#define A13 67
#define A14 68
#define A15 69

// --- MACROT ---
#define F(string_literal) (string_literal)
#define PROGMEM
// bitRead ym. apufunktiot jos tarvitset
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))

// --- SERIAL ---
class MockSerial {
public:
  void begin(long baud) {
    std::cout << "[Serial] Initialized at " << baud << " baud" << std::endl;
  }
  // Template hoitaa kaikki print tyypit kerralla!
  template <typename T> void print(T val) { std::cout << val; }

  template <typename T> void println(T val) { std::cout << val << std::endl; }

  void println() { std::cout << std::endl; }
};

extern MockSerial Serial;

// --- TIME ---
inline std::chrono::steady_clock::time_point getStartTime() {
  static auto start = std::chrono::steady_clock::now();
  return start;
}

inline unsigned long millis() {
  auto now = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      now - getStartTime());
  return static_cast<unsigned long>(duration.count());
}

inline unsigned long micros() {
  auto now = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      now - getStartTime());
  return static_cast<unsigned long>(duration.count());
}

inline void delay(unsigned long ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

inline void delayMicroseconds(unsigned int us) {
  std::this_thread::sleep_for(std::chrono::microseconds(us));
}

// --- IO ---
inline void pinMode(uint8_t pin, uint8_t mode) {
  // Ei tehdä mitään, tai voidaan logata
}

inline void digitalWrite(uint8_t pin, uint8_t value) {}

inline int digitalRead(uint8_t pin) {
  return HIGH; // Oletuksena napit ylhäällä (INPUT_PULLUP)
}

inline int analogRead(uint8_t pin) {
  // palauttaa keskikohdan jotta joystick ei "vaella" itsekseen.
  return 512;
}

inline long random(long min, long max) { return min + (rand() % (max - min)); }

inline long random(long max) {
  if (max == 0)
    return 0;
  return rand() % max;
}

inline void randomSeed(unsigned long seed) { srand(seed); }

// Arduinon map-funktion toteutus PC:lle
inline long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template <typename T> inline T min(T a, T b) { return (a < b) ? a : b; }

template <typename T> inline T max(T a, T b) { return (a > b) ? a : b; }
