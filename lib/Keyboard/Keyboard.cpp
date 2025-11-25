#include "Keyboard.h"

#define R1 49
#define R2 48
#define R3 47
#define R4 46
#define C1 42
#define C2 43
#define C3 44
#define C4 45

static constexpr uint8_t KEYBOARD_ROWS = 4;
static constexpr uint8_t KEYBOARD_COLS = 4;

static constexpr char keys[KEYBOARD_ROWS * KEYBOARD_COLS] = {
    '1', '2', '3', 'A', '4', '5', '6', 'B',
    '7', '8', '9', 'C', '*', '0', '#', 'D'};

static constexpr uint8_t rowPins[KEYBOARD_ROWS] = {R1, R2, R3, R4};
static constexpr uint8_t colPins[KEYBOARD_COLS] = {C1, C2, C3, C4};

static const uint8_t DEBOUNCE_COOLDOWN_MS = 50;

Keyboard::Keyboard()
    : keypad((uint8_t *)keys, rowPins, colPins, KEYBOARD_ROWS, KEYBOARD_COLS) {
  keypad.begin();
}

void Keyboard::update() {
  keypad.tick();

  while (keypad.available()) {
    keypadEvent e = keypad.read();

    const uint32_t currentTime = millis();

    if ((currentTime - lastKeyPressTime) < DEBOUNCE_COOLDOWN_MS) {
      continue;
    }

    lastKeyPressTime = currentTime;

    KeyEvent ev;
    ev.key = (char)e.bit.KEY;
    ev.type = (e.bit.EVENT == KEY_JUST_PRESSED) ? KeyEvent::Type::PRESS
                                                : KeyEvent::Type::RELEASE;

    uint8_t next = (head + 1) % MAX_EVENTS;
    if (next != tail) {
      buffer[head] = ev;
      head = next;
    }
  }
}

bool Keyboard::hasEvent() const { return head != tail; }

Keyboard::KeyEvent Keyboard::nextEvent() {
  KeyEvent e = buffer[tail];
  tail = (tail + 1) % MAX_EVENTS;
  return e;
}
