#pragma once

#include <iostream>

enum class KeyCode {
  Space = 32,
  A = 65,
  D = 68,
  R = 82,
  S = 83,
  W = 87,
  Escape = 256,
  Enter = 257,
  Tab = 258,
  Right = 262,
  Left = 263,
  Down = 264,
  Up = 265
};

enum class MouseCode {
  ButtonLeft = 0,
  ButtonRight = 1,
  ButtonMiddle = 2,
  Button3 = 3,
  Button4 = 4,
  Button5 = 5,
  Button6 = 6,
  Button7 = 7,
};

inline std::ostream &operator<<(std::ostream &os, KeyCode keyCode) {
  os << static_cast<int>(keyCode);
  return os;
}

inline std::ostream &operator<<(std::ostream &os, MouseCode mouseCode) {
  os << static_cast<int>(mouseCode);
  return os;
}
