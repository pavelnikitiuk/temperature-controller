#ifndef TELEGRAM_MANAGER_H
#define TELEGRAM_MANAGER_H

#include <FastBot2.h>
#include <map>

#include "Variables.h"

enum TelegramEvent {
  TELEGRAM_ENABLE,
  TELEGRAM_DISABLE,
  TELEGRAM_MODE,
  TELEGRAM_TEMPERATURE,
  TELEGRAM_SHOW_SETTINGS,
  TELEGRAM_CHANGE_SETTINGS,
  TELEGRAM_SHOW_POWER,
  TELEGRAM_SHOW_TOTAL_POWER,
  TELEGRAM_CHOSE_CHANGE_ON_TEMPERATURE,
  TELEGRAM_CHOSE_CHANGE_OFF_TEMPERATURE,
  TELEGRAM_CHOSE_EXIT_FROM_SETTINGS,
};

class TelegramManager {
public:
  TelegramManager();
  ~TelegramManager();

  void begin(const char *token, const char chatId[128]);
  void handle();
  void sendRelayStateChanged(bool status);
  void sendTemperature(float temperature);
  void sendRelayModeChanged(RelayControlMode mode);
  void sendTemperatureOnChanged(float temperature);
  void sendTemperatureOffChanged(float temperature);
  void sendShowSettings(float temperatureOn, float temperatureOff);
  void sendInvalidTemperature();
  void onMessage(TelegramEvent, std::function<void()> callback);
  void onMessage(std::function<void(Text)> callback);
  void sendMessage(su::Text &text);
  void sendMessage(const char *str);
  void sendMessage(const String &str);
  void sendCurrentPower(float power, float current);
  void sendTotalPower(float power);
  int sendInlineMenu();
  int updateInlineMenu(TelegramSettingsMenuState state);

private:
  void handleUpdate(fb::Update &update);
  fb::Menu getReplyKeyboard();
  FastBot2 *bot;
  const char *chatId;
  const unsigned long updateInterval = 5000;
  unsigned long lastUpdateTime = 0;
  void handleMessage(fb::MessageRead message);
  void handleQuery(fb::QueryRead query);
  std::map<TelegramEvent, std::function<void()>> callbacks;
  std::function<void(Text)> everyMessageCallback = [](Text) {};
  void sendCallback(TelegramEvent event);
  fb::InlineMenu getInlineMenuWithTemperatureAsk();
  fb::InlineMenu getInlineMenuWithOffTemperatureAsk();
  fb::InlineMenu getInlineMenuWithOnTemperatureAsk();
  void closeInlineMenu();
  const String getTemperatureSettings(float temperatureOn,
                                      float temperatureOff);
};

#endif
