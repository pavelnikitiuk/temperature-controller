#ifndef MODE_SETTINGS_SCREEN_H
#define MODE_SETTINGS_SCREEN_H

#include "StaticScreen.h"
#include "Variables.h"
#include <U8g2lib.h>

class ModeSettingScreen : public StaticScreen {
public:
  ModeSettingScreen(U8G2 &u8g2) : StaticScreen(u8g2) {}
  void drawScreen();
  void onClick() override;

protected:
  unsigned int getUpdateTime() override;

private:
  bool shouldUpdate();
  unsigned int updateTime = 200;
};
#endif