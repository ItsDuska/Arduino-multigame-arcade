#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class String {
public:
  // Käytämme std::stringiä sisäisenä varastona
  std::string _str;

  // --- KONSTRUKTORIT ---
  String(const char *s = "") : _str(s ? s : "") {}
  String(std::string s) : _str(s) {}
  String(const String &s) : _str(s._str) {}
  String(char c) : _str(1, c) {}
  String(int n) : _str(std::to_string(n)) {}
  String(unsigned int n) : _str(std::to_string(n)) {}
  String(long n) : _str(std::to_string(n)) {}
  String(unsigned long n) : _str(std::to_string(n)) {}
  String(float n) : _str(std::to_string(n)) {}
  String(double n) : _str(std::to_string(n)) {}

  // --- OMINAISUUDET ---
  unsigned int length() const { return _str.length(); }
  const char *c_str() const { return _str.c_str(); }

  // --- MUUNNOKSET ---
  long toInt() const {
    try {
      return std::stol(_str);
    } catch (...) {
      return 0;
    }
  }

  float toFloat() const {
    try {
      return std::stof(_str);
    } catch (...) {
      return 0.0f;
    }
  }

  // --- OPERAATTORIT (=) ---
  String &operator=(const String &rhs) {
    _str = rhs._str;
    return *this;
  }
  String &operator=(const char *rhs) {
    _str = rhs;
    return *this;
  }

  // --- YHDISTÄMINEN (+=) ---
  String &operator+=(const String &rhs) {
    _str += rhs._str;
    return *this;
  }
  String &operator+=(const char *rhs) {
    _str += rhs;
    return *this;
  }
  String &operator+=(char rhs) {
    _str += rhs;
    return *this;
  }
  String &operator+=(int rhs) {
    _str += std::to_string(rhs);
    return *this;
  }

  // --- YHDISTÄMINEN (+) ---
  friend String operator+(const String &lhs, const String &rhs) {
    return String(lhs._str + rhs._str);
  }
  friend String operator+(const String &lhs, const char *rhs) {
    return String(lhs._str + rhs);
  }
  friend String operator+(const char *lhs, const String &rhs) {
    return String(std::string(lhs) + rhs._str);
  }

  // --- VERTAILU (==, !=) ---
  bool operator==(const String &rhs) const { return _str == rhs._str; }
  bool operator==(const char *rhs) const { return _str == rhs; }
  bool operator!=(const String &rhs) const { return _str != rhs._str; }

  // --- INDEKSOINTI [] ---
  char operator[](unsigned int index) const {
    if (index >= length())
      return 0;
    return _str[index];
  }

  // --- MUOKKAUS ---
  void toUpperCase() {
    std::transform(_str.begin(), _str.end(), _str.begin(), ::toupper);
  }

  void toLowerCase() {
    std::transform(_str.begin(), _str.end(), _str.begin(), ::tolower);
  }

  // --- HAKU JA LEIKKAUS ---
  int indexOf(char c) const {
    size_t pos = _str.find(c);
    return (pos == std::string::npos) ? -1 : (int)pos;
  }

  String substring(unsigned int left, unsigned int right) const {
    if (left >= length())
      return String("");
    if (right > length())
      right = length();
    if (left > right)
      return String("");
    return String(_str.substr(left, right - left));
  }

  String substring(unsigned int left) const {
    if (left >= length())
      return String("");
    return String(_str.substr(left));
  }

  // Stream output (jotta std::cout << String toimii)
  friend std::ostream &operator<<(std::ostream &os, const String &s) {
    os << s._str;
    return os;
  }
};
