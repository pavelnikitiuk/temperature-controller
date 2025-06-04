#include "StaticScreen.h"

void StaticScreen::draw(bool force) {
  if (!shouldUpdate() && !force) {
    return;
  }
  u8g2.firstPage();
  do {
    drawScreen();
  } while (u8g2.nextPage());
}

unsigned int StaticScreen::getUpdateTime() { return 1000; }