#ifndef ANIMATED_SCREEN_H
#define ANIMATED_SCREEN_H
#include "Screen.h"

class AnimatedScreen : public Screen {
public:
  AnimatedScreen(U8G2 &u8g2) : Screen(u8g2) {}
  void draw() override;

private:
  virtual void drawFrame(int frame) {};
  virtual int getFramesCount() { return 0; };
  byte currentFrame = 0;
  unsigned int getUpdateTime() override;
};

#endif