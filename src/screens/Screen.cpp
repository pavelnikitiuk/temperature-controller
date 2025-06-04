#include "Screen.h"

void Screen::handle(bool force) {
  unsigned long currentTime = millis();
  if (currentTime - lastFrameTime >= getUpdateTime()) {
    lastFrameTime = currentTime;
    draw(force);
  }
}