#ifndef INFO_SCREEN_H
#define INFO_SCREEN_H

#include "StaticScreen.h"
#include "Variables.h"
#include <U8g2lib.h>
#include <qrcode.h>

enum InfoScreenViewMode {
  INFO_SCREEN_TEXT,
  INFO_SCREEN_QR,
  INFO_SCREEN_LAST
};

class InfoScreen : public StaticScreen {
public:
  InfoScreen(U8G2 &u8g2) : StaticScreen(u8g2) {}
  void drawScreen();
  void drawText();
  void drawQr();
  void onClick() override;
private:
  bool shouldUpdate();
  WiFiState currentState;
  InfoScreenViewMode mode = INFO_SCREEN_TEXT;
};

#endif