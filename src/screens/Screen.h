#ifndef SCREEN_H
#define SCREEN_H

#include <U8g2lib.h>

class Screen {
public:
  Screen(U8G2 &u8g2) : u8g2(u8g2) {}
  virtual void draw(bool force) = 0;
  void handle(bool force);
  virtual ~Screen() = default;

protected:
  virtual unsigned int getUpdateTime();

private:
  unsigned long lastFrameTime = 0;

protected:
  U8G2 &u8g2;
};

#endif
