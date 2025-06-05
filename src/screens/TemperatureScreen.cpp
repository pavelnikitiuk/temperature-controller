#include "TemperatureScreen.h"

void TemperatureScreen::drawFrame(int frame) {
  float temperature = globalState.temperature;
  u8g2.setFont(u8g2_font_open_iconic_play_8x_t);
  char tempStr[32];
  snprintf(tempStr, sizeof(tempStr), "%.1f", temperature);
  u8g2.drawStr(30, 30, tempStr);
};

int TemperatureScreen::getFramesCount() {
  return 12;
}