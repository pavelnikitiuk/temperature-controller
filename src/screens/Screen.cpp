#include "Screen.h"

void Screen::handle() {
  unsigned long currentTime = millis();
  if (currentTime - lastFrameTime >= getUpdateTime()) {
    lastFrameTime = currentTime;
    draw();
  }
}

unsigned int Screen::getUpdateTime() {
  return 1000;
}

Screen::~Screen() = default;