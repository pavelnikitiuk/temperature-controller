
#ifndef POWER_SCREEN_H
#define POWER_SCREEN_H

#include <U8g2lib.h>
#include "Variables.h"
#include "StaticScreen.h"

enum PowerScreenState {
  POWER_SCREEN_STATE_CURRENT_AND_POWER,
  POWER_SCREEN_STATE_SUMMARY_POWER,
  POWER_SCREEN_STATE_LAST
};

class PowerScreen : public StaticScreen {
public:
  PowerScreen(U8G2 &u8g2): StaticScreen(u8g2) {};
  void drawScreen();
  void onClick() override;
  void onHold() override;
private:
  PowerScreenState state = POWER_SCREEN_STATE_CURRENT_AND_POWER;
  void drawCurrentAndPower();
  void drawSummaryPower();
};

#endif
