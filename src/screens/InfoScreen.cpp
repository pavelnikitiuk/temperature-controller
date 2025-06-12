#include "InfoScreen.h"

void InfoScreen::drawScreen() {
  if (mode == INFO_SCREEN_TEXT) {
    drawText();
  }
  if (mode == INFO_SCREEN_QR) {
    drawQr();
  }
}

void InfoScreen::drawQr() {
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];

  char url[64];
  snprintf(url, sizeof(url), "http://%s", currentState.ip.toString().c_str());

  qrcode_initText(&qrcode, qrcodeData, 3, 0, url);

  int scale = min(OLED_WIDTH / qrcode.size, OLED_HEIGHT / qrcode.size);
  int shiftX = (OLED_WIDTH - qrcode.size * scale) / 2;
  int shiftY = (OLED_HEIGHT - qrcode.size * scale) / 2;

  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      if (qrcode_getModule(&qrcode, x, y)) {
        u8g2.drawBox(shiftX + x * scale, shiftY + y * scale, scale, scale);
      }
    }
  }
}

void InfoScreen::drawText() {
  currentState = globalState.wifiState;

  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.drawHLine(0, 14, 128);

  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(4, 11, "WiFi INFO");

  const char *modeStr =
      currentState.mode == WIFI_MODE ? "Mode: WIFI" : "Mode: AP";
  u8g2.drawStr(4, 26, modeStr);

  u8g2.drawStr(4, 38, "SSID:");
  u8g2.drawStr(40, 38, currentState.name.c_str());

  String ipStr = currentState.ip.toString();
  u8g2.drawStr(4, 50, "IP:");
  u8g2.drawStr(40, 50, ipStr.c_str());
}

bool InfoScreen::shouldUpdate() {
  WiFiState nextState = globalState.wifiState;
  return nextState.apPassword != currentState.apPassword ||
         nextState.ip != currentState.ip ||
         nextState.mode != currentState.mode ||
         nextState.name != currentState.name;
}

void InfoScreen::onClick() {
  mode = static_cast<InfoScreenViewMode>((mode + 1) % INFO_SCREEN_LAST);
}
