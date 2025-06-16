#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include "StaticScreen.h"
#include "Variables.h"
#include <U8g2lib.h>

class MainScreen : public StaticScreen {
public:
  MainScreen(U8G2 &u8g2) : StaticScreen(u8g2) {}
  void drawScreen();
  void onClick() override;
};
#endif