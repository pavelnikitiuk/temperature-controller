#ifndef WIFI_SCREEN_H
#define WIFI_SCREEN_H

#include <U8g2lib.h>
#include "AnimatedScreen.h"

class WiFiScreen : public AnimatedScreen {
public:
  WiFiScreen(U8G2 &u8g2) : AnimatedScreen(u8g2) {}

private:
  void drawFrame(int frame) override;
  int getFramesCount() override;
};
#endif