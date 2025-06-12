#ifndef TEMPERATURE_SETTINGS_SCREEN_H
#define TEMPERATURE_SETTINGS_SCREEN_H

#include "StaticScreen.h"
#include "Variables.h"
#include <U8g2lib.h>

struct TemperatureSettingScreenState {
  bool isEditing = false;
  bool isEditingOn = false;
  bool isEditingOff = false;
  bool showEditingValue = true;
  unsigned int lastBlinkSwitchTime = 0;
  RelayControl relayControl;
};

class TemperatureSettingScreen : public StaticScreen {
public:
  TemperatureSettingScreen(U8G2 &u8g2) : StaticScreen(u8g2) {}
  void drawScreen();
  void onClick() override;
  void onHold() override;
  bool onLeftScroll() override;
  bool onRightScroll() override;

protected:
  unsigned int getUpdateTime() override;

private:
  TemperatureSettingScreenState state;
  bool shouldUpdate();
  unsigned int updateTime = 200;
  void adjustTemperature(float delta);
  float delta = 0.5;
};
#endif