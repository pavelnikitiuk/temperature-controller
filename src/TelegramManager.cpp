#include "TelegramManager.h"

TelegramManager::TelegramManager() : bot(nullptr), chatId(nullptr) {}

TelegramManager::~TelegramManager() {
  if (bot != nullptr) {
    delete bot;
  }
}

void TelegramManager::begin(const char *token, const char *chat) {
  if (bot != nullptr) {
    delete bot;
  }
  bot = new FastBot2(token);
  chatId = chat;
  bot->onUpdate([this](fb::Update &update) { handleUpdate(update); });
  // fb::Message msg("Send inline menu", chatId);
  // fb::InlineMenu menu(
  //     "test 1 ; test 2 ; test 3 \n test 4 ; test 5",
  //     "test1;test2;test3;https://www.google.ru/;https://www.yandex.ru/");
  // msg.setInlineMenu(menu);
  // bot->sendMessage(msg);
  fb::Message message(I18N_TELEGRAM_OPEN_MENU, chatId);
  // drawReplyKeyboard(message);
}

void TelegramManager::handle() {
  if (bot == nullptr) {
    return;
  }
  bot->tick();
}

void TelegramManager::sendMessage(su::Text &text) {
  if (bot == nullptr) {
    return;
  }
  fb::Message message(text, chatId);
  fb::Menu menu = getReplyKeyboard();
  message.setMenu(menu);
  bot->sendMessage(message, false);
}

void TelegramManager::sendMessage(const char *str) {
  su::Text txt(str);
  sendMessage(txt);
}

void TelegramManager::sendMessage(const String &str) {
  su::Text txt(str);
  sendMessage(txt);
}

void TelegramManager::sendTemperature(float temperature) {
  sendMessage(String(I18N_TELEGRAM_TEMPERATURE) + ": " +
              globalState.view.temperature);
}

fb::Menu TelegramManager::getReplyKeyboard() {
  bool isEnabled = globalState.configuration.isRelayEnabled;
  bool isModeManual =
      globalState.configuration.relayControl.mode == RELAY_CONTROL_MANUAL;
  fb::Menu menu;
  menu.resize = 1;
  menu.persistentDefault = 1;
  String onIcon = isEnabled && isModeManual ? " ✅" : "";
  String offIcon = !isEnabled && isModeManual ? " ☑️" : "";
  String modeIcon = "";
  if (!isModeManual) {
    modeIcon = isEnabled ? " ✅" : " ☑️";
  }

  menu.addButton(String(I18N_TELEGRAM_SET_RELAY_ON) + onIcon);
  menu.addButton(String(I18N_TELEGRAM_SET_RELAY_OFF) + offIcon);
  menu.newRow();
  menu.addButton(String(I18N_TELEGRAM_MODE_AUTO) + modeIcon);
  menu.addButton(String(I18N_TELEGRAM_TEMPERATURE));
  menu.newRow();
  menu.addButton(I18N_TELEGRAM_SHOW_SETTING);
  menu.addButton(I18N_TELEGRAM_CHANGE_SETTING);

  return menu;
}

void TelegramManager::sendRelayModeChanged(RelayControlMode mode) {
  if (mode == RELAY_CONTROL_MANUAL) {
    sendMessage(I18N_TELEGRAM_MODE_AUTO_ENABLE);
  } else {
    sendMessage(I18N_TELEGRAM_MODE_AUTO_DISABLE);
  }
}

void TelegramManager::onMessage(TelegramEvent event,
                                std::function<void()> callback) {
  callbacks[event] = callback;
}

void TelegramManager::sendRelayStateChanged(bool state) {
  if (state) {
    sendMessage(I18N_TELEGRAM_ENABLE_RELAY);
  } else {
    sendMessage(I18N_TELEGRAM_DISABLE_RELAY);
  }
}

void TelegramManager::sendCallback(TelegramEvent event) {
  if (!callbacks.count(event)) {
    return;
  }
  callbacks[event]();
}

void TelegramManager::handleMessage(fb::MessageRead message) {
  if (message.chat().id() != chatId) {
    return;
  }
  fb::Message messageToSend;
  messageToSend.chatID = chatId;
  switch (message.text().hash()) {
  case SH(I18N_TELEGRAM_TEMPERATURE): {
    sendCallback(TELEGRAM_TEMPERATURE);
    break;
  }
  case SH(I18N_TELEGRAM_SET_RELAY_ON_ENABLED):
  case SH(I18N_TELEGRAM_SET_RELAY_ON): {
    sendCallback(TELEGRAM_ENABLE);
    break;
  }
  case SH(I18N_TELEGRAM_SET_RELAY_OFF_ENABLED):
  case SH(I18N_TELEGRAM_SET_RELAY_OFF): {
    sendCallback(TELEGRAM_DISABLE);
    break;
  }
  case SH(I18N_TELEGRAM_MODE_AUTO_ENABLED):
  case SH(I18N_TELEGRAM_MODE_AUTO_DISABLED):
  case SH(I18N_TELEGRAM_MODE_AUTO): {
    sendCallback(TELEGRAM_MODE);
    break;
  }
  case SH(I18N_TELEGRAM_SHOW_SETTING): {
    sendCallback(TELEGRAM_SHOW_SETTINGS);
    break;
  }
  default:
    break;
  }
}

void TelegramManager::handleUpdate(fb::Update &update) {
  if (update.isMessage()) {
    handleMessage(update.message());
  }
}

void TelegramManager::sendTemperatureOnChanged(float temperature) {
  sendMessage(String(I18N_TELEGRAM_ON_TEMPERATURE_CHANGED) + temperature);
}
void TelegramManager::sendTemperatureOffChanged(float temperature) {
  sendMessage(String(I18N_TELEGRAM_OFF_TEMPERATURE_CHANGED) + temperature);
}
void TelegramManager::sendShowSettings(float temperatureOn,
                                       float temperatureOff) {
  sendMessage(String(I18N_TELEGRAM_ON_TEMPERATURE) + temperatureOn + "\n" +
              String(I18N_TELEGRAM_OFF_TEMPERATURE) + temperatureOff);
}