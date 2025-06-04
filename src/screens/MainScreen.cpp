#include "MainScreen.h"

void MainScreen::drawScreen() {
  currentState.isRelayEnabled = globalState.isRelayEnabled;
  currentState.temperature = globalState.temperature;
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(0, 10, "System Info");

  char tempStr[32];
  snprintf(tempStr, sizeof(tempStr), "Temp: %.1f C", globalState.temperature);
  u8g2.drawStr(0, 24, tempStr);

  const char *relayStr =
      globalState.isRelayEnabled ? "Relay: ON" : "Relay: OFF";
  u8g2.drawStr(0, 38, relayStr);
};

bool MainScreen::shouldUpdate() {
  return globalState.temperature != currentState.temperature ||
         globalState.isRelayEnabled != currentState.isRelayEnabled;
}