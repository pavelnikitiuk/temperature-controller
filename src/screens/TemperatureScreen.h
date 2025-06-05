#ifndef TEMPERATURE_SCREEN_H
#define TEMPERATURE_SCREEN_H

#include <U8g2lib.h>
#include "AnimatedScreen.h"
#include "Variables.h"

class TemperatureScreen : public AnimatedScreen {
public:
  TemperatureScreen(U8G2 &u8g2) : AnimatedScreen(u8g2) {}

private:
  void drawFrame(int frame) override;
  int getFramesCount() override;
};
#endif