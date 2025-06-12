#ifndef STATIC_SCREEN_H
#define STATIC_SCREEN_H
#include "Screen.h"

class StaticScreen : public Screen {
public:
  StaticScreen(U8G2 &u8g2) : Screen(u8g2) {}
  void draw() override;

private:
  unsigned int getUpdateTime() override;
  virtual void drawScreen() {};
  virtual bool shouldUpdate() { return false; };
};

#endif