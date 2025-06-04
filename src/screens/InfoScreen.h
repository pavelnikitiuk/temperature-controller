#ifndef INFO_SCREEN_H
#define INFO_SCREEN_H

#include "StaticScreen.h"
#include "Variables.h"
#include <U8g2lib.h>

class InfoScreen : public StaticScreen {
public:
  InfoScreen(U8G2 &u8g2) : StaticScreen(u8g2) {}
  void drawScreen();

private:
  bool shouldUpdate();
  WiFiState currentState;
};

#endif