#include "InfoScreen.h"

void InfoScreen::drawScreen() {
  currentState = globalState.wifiState;
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(10, 10, "WiFi Info");

  const char *modeStr =
      currentState.mode == WIFI_MODE ? "Mode: WIFI" : "Mode: AP";
  u8g2.drawStr(0, 22, modeStr);

  u8g2.drawStr(0, 34, "SSID:");
  u8g2.drawStr(35, 34, currentState.name.c_str());

  String ipStr = currentState.ip.toString();
  u8g2.drawStr(0, 46, "IP:");
  u8g2.drawStr(35, 46, ipStr.c_str());
};

bool InfoScreen::shouldUpdate() {
  WiFiState nextState = globalState.wifiState;
  return nextState.apPassword != currentState.apPassword ||
         nextState.ip != currentState.ip ||
         nextState.mode != currentState.mode ||
         nextState.name != currentState.name;
}
