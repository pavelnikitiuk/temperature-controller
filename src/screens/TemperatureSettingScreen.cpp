#include "TemperatureSettingScreen.h"

const uint8_t *selectFontToFitWidth(const char *str, const uint8_t *fonts[],
                                    size_t fontCount, int maxWidth, U8G2 u8g2) {
  for (size_t i = 0; i < fontCount; ++i) {
    u8g2.setFont(fonts[i]);
    if (u8g2.getStrWidth(str) <= maxWidth) {
      return fonts[i];
    }
  }
  return fonts[fontCount - 1];
}

void TemperatureSettingScreen::drawScreen() {
  unsigned long now = millis();
  unsigned long interval = state.showEditingValue ? updateTime * 4 : updateTime;

  if (now - state.lastBlinkSwitchTime >= interval) {
    state.lastBlinkSwitchTime = now;
    state.showEditingValue = !state.showEditingValue;
  }

  state.relayControl = globalState.relayControl;
  float tempOn = state.relayControl.temperatureOn;
  float tempOff = state.relayControl.temperatureOff;

  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.drawVLine(63, 0, 64);
  u8g2.drawVLine(64, 0, 64);

  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(4, 12, "ON");
  u8g2.drawStr(68, 12, "OFF");

  const uint8_t *fonts[] = {u8g2_font_logisoso24_tf, u8g2_font_logisoso22_tf,
                            u8g2_font_logisoso18_tf, u8g2_font_logisoso16_tf};
  size_t fontCount = sizeof(fonts) / sizeof(fonts[0]);

  char onStr[6];
  snprintf(onStr, sizeof(onStr), "%.1f", tempOn);
  const uint8_t *fontOn =
      selectFontToFitWidth(onStr, fonts, fontCount, 64, u8g2);
  u8g2.setFont(fontOn);
  int16_t xOn = (64 - u8g2.getStrWidth(onStr)) / 2;
  u8g2.setCursor(xOn, 50);
  if (!state.isEditing || !state.isEditingOn ||
      (state.showEditingValue && state.isEditingOn)) {
    u8g2.print(onStr);
  }

  char offStr[6];
  snprintf(offStr, sizeof(offStr), "%.1f", tempOff);
  const uint8_t *fontOff =
      selectFontToFitWidth(offStr, fonts, fontCount, 64, u8g2);
  u8g2.setFont(fontOff);
  int16_t xOff = 64 + (64 - u8g2.getStrWidth(offStr)) / 2;
  u8g2.setCursor(xOff, 50);
  if (!state.isEditing || !state.isEditingOff ||
      (state.showEditingValue && state.isEditingOff)) {
    u8g2.print(offStr);
  }
}

bool TemperatureSettingScreen::shouldUpdate() {
  return state.isEditing || globalState.relayControl != state.relayControl;
}

void TemperatureSettingScreen::onClick() {
  if (!state.isEditing) {
    return;
  }
  if (state.isEditingOn) {
    state.isEditingOn = false;
    state.isEditingOff = true;
    return;
  }
  state.isEditingOn = false;
  state.isEditingOff = false;
  state.isEditing = false;
}

void TemperatureSettingScreen::onHold() {
  if (state.isEditing) {
    state.isEditing = false;
    state.isEditingOff = false;
    state.isEditingOn = false;
  } else {
    state.isEditing = true;
    state.isEditingOff = false;
    state.isEditingOn = true;
  }
}

bool TemperatureSettingScreen::onLeftScroll() {
  if (!state.isEditing) {
    return true;
  }

  adjustTemperature(-delta);
  return false;
}

bool TemperatureSettingScreen::onRightScroll() {
  if (!state.isEditing) {
    return true;
  }

  adjustTemperature(delta);
  return false;
}

void TemperatureSettingScreen::adjustTemperature(float delta) {
  if (state.isEditingOn) {
    globalState.relayControl.temperatureOn += delta;
  } else {
    globalState.relayControl.temperatureOff += delta;
  }
}

unsigned int TemperatureSettingScreen::getUpdateTime() { return 200; }
