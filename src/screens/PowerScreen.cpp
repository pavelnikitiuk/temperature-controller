#include "PowerScreen.h"

void PowerScreen::drawScreen() {
  if (state == POWER_SCREEN_STATE_CURRENT_AND_POWER) {
    drawCurrentAndPower();
  } else {
    drawSummaryPower();
  }
}

void PowerScreen::drawSummaryPower() {
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.setFont(I18N_FONT_TITLE);
  u8g2.drawUTF8(4, 12, I18N_SCREEN_POWER_SUM);

  float totalEnergyWh = globalState.configuration.watts;
  float displayValue = totalEnergyWh / 1000;

  char energyStr[10];
  snprintf(energyStr, sizeof(energyStr), displayValue < 100 ? "%.2f" : "%.0f",
           displayValue);

  const uint8_t *fonts[] = {u8g2_font_logisoso38_tf, u8g2_font_logisoso32_tf,
                            u8g2_font_logisoso26_tf, u8g2_font_logisoso22_tf};

  const uint8_t *selectedFont = fonts[0];
  int16_t textWidth = 0;

  for (uint8_t i = 0; i < sizeof(fonts) / sizeof(fonts[0]); i++) {
    u8g2.setFont(fonts[i]);
    textWidth = u8g2.getStrWidth(energyStr);
    if (textWidth <= 120) {
      selectedFont = fonts[i];
      break;
    }
  }

  u8g2.setFont(selectedFont);
  int16_t x = (128 - textWidth) / 2;
  u8g2.setCursor(x, 52);
  u8g2.print(energyStr);
}

void PowerScreen::drawCurrentAndPower() {
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.drawVLine(63, 0, 64);
  u8g2.drawVLine(64, 0, 64);

  u8g2.setFont(I18N_FONT_TITLE);
  u8g2.drawUTF8(4, 12, I18N_SCREEN_POWER_WATTS);
  u8g2.drawUTF8(68, 12, I18N_SCREEN_POWER_AMPERES);

  char powerStr[8];
  snprintf(powerStr, sizeof(powerStr), "%.1f", globalState.view.power);

  char currentStr[8];
  snprintf(currentStr, sizeof(currentStr), "%.2f", globalState.view.current);

  const uint8_t *fonts[] = {u8g2_font_logisoso24_tf, u8g2_font_logisoso22_tf,
                            u8g2_font_logisoso18_tf, u8g2_font_logisoso16_tf};

  const uint8_t *selectedFont = fonts[3];
  int16_t textWidth = 0;

  for (uint8_t i = 0; i < sizeof(fonts) / sizeof(fonts[0]); i++) {
    u8g2.setFont(fonts[i]);
    textWidth = u8g2.getStrWidth(powerStr);
    if (textWidth <= 64) {
      selectedFont = fonts[i];
      break;
    }
  }

  u8g2.setFont(selectedFont);
  int16_t x = (64 - textWidth) / 2;
  u8g2.setCursor(x, 50);
  u8g2.print(powerStr);

  u8g2.setFont(u8g2_font_logisoso24_tf);
  int16_t currentWidth = u8g2.getStrWidth(currentStr);
  int16_t currentX = 64 + (64 - currentWidth) / 2;
  u8g2.setCursor(currentX, 50);
  u8g2.print(currentStr);
}

void PowerScreen::onClick() {
  state = static_cast<PowerScreenState>((state + 1) % POWER_SCREEN_STATE_LAST);
}

void PowerScreen::onHold() { globalState.configuration.watts = 0; }
