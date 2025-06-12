#ifndef SCREEN_H
#define SCREEN_H

#include <U8g2lib.h>

class Screen {
public:
  Screen(U8G2 &u8g2) : u8g2(u8g2) {}

  virtual void draw() = 0;
  void handle();

  virtual bool onLeftScroll() { return true; }
  virtual bool onRightScroll() { return true; }
  virtual void onClick() {}
  virtual void onHold() {}
  virtual void screenWillRemove() {}
  virtual void screenDidAppear() {}

  virtual ~Screen();

protected:
  virtual unsigned int getUpdateTime();
  U8G2 &u8g2;

private:
  unsigned long lastFrameTime = 0;
};

#endif
