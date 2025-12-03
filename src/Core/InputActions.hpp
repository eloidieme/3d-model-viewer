#pragma once

enum class Action {
  None = 0,
  MoveForward,
  MoveBackward,
  MoveLeft,
  MoveRight,
  MoveUp,
  MoveDown,
  ToggleCursor,
  ReloadShader
};