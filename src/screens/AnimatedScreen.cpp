#include "AnimatedScreen.h"

void AnimatedScreen::draw() {
  u8g2.firstPage();
  do {
    drawFrame(currentFrame);
  } while (u8g2.nextPage());

  currentFrame = (currentFrame + 1) % getFramesCount();
}

unsigned int AnimatedScreen::getUpdateTime() { return 50; }
