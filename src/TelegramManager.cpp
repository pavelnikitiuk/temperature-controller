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

  fb::Message message(I18N_TELEGRAM_OPEN_MENU, chatId);
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

void TelegramManager::onMessage(std::function<void(Text)> callback) {
  everyMessageCallback = callback;
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

void TelegramManager::sendInvalidTemperature() {
  sendMessage(I18N_TELEGRAM_INVALID_TEMPERATURE);
}

void TelegramManager::handleMessage(fb::MessageRead message) {
  if (message.chat().id() != chatId) {
    return;
  }
  everyMessageCallback(message.text());
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
  case SH(I18N_TELEGRAM_CHANGE_SETTING): {
    sendCallback(TELEGRAM_CHANGE_SETTINGS);
    break;
  }
  default:
    break;
  }
}

void TelegramManager::handleQuery(fb::QueryRead query) {
  if (query.message().chat().id() != chatId) {
    return;
  }
  switch (query.data().hash()) {
  case SH(TELEGRAM_EXIT_BUTTON): {
    sendCallback(TELEGRAM_CHOSE_EXIT_FROM_SETTINGS);
    break;
  }
  case SH(TELEGRAM_CHANGE_ON_BUTTON): {
    sendCallback(TELEGRAM_CHOSE_CHANGE_ON_TEMPERATURE);
    break;
  }
  case SH(TELEGRAM_CHANGE_OFF_BUTTON): {
    sendCallback(TELEGRAM_CHOSE_CHANGE_OFF_TEMPERATURE);
    break;
  }

  default:
    break;
  }
  bot->answerCallbackQuery(query.id());
}

void TelegramManager::handleUpdate(fb::Update &update) {
  if (update.isMessage()) {
    handleMessage(update.message());
  }
  if (update.isQuery()) {
    handleQuery(update.query());
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
  sendMessage(getTemperatureSettings(temperatureOn, temperatureOff));
}

const String TelegramManager::getTemperatureSettings(float temperatureOn,
                                                     float temperatureOff) {
  return String(I18N_TELEGRAM_ON_TEMPERATURE) + temperatureOn + "\n" +
         String(I18N_TELEGRAM_OFF_TEMPERATURE) + temperatureOff;
}

int TelegramManager::sendInlineMenu() {
  if (bot == nullptr) {
    return 0;
  }
  if (globalState.configuration.telegramSettingsMenu.messageId) {
    closeInlineMenu();
  }
  float temperatureOn = globalState.configuration.relayControl.temperatureOn;
  float temperatureOff = globalState.configuration.relayControl.temperatureOff;
  fb::Message message(String(I18N_TELEGRAM_INLINE_MENU_TITLE) + "\n" +
                          getTemperatureSettings(temperatureOn, temperatureOff),
                      chatId);
  fb::InlineMenu menu = getInlineMenuWithTemperatureAsk();
  message.setInlineMenu(menu);
  bot->sendMessage(message);
  int messageId = bot->lastBotMessage();
  Serial.print("Message created: ");

  Serial.println(messageId);

  globalState.configuration.telegramSettingsMenu.messageId = messageId;
  return messageId;
}

int TelegramManager::updateInlineMenu(TelegramSettingsMenuState state) {
  fb::InlineMenu menu;
  fb::TextEdit text;
  int messageId = globalState.configuration.telegramSettingsMenu.messageId;
  Serial.println(state);
  switch (state) {
  case TELEGRAM_SETTINGS_ASK_TEMPERATURE_TYPE:
    sendInlineMenu();
    return messageId;
  case TELEGRAM_SETTINGS_ASK_TEMPERATURE_ON:
    menu = getInlineMenuWithOnTemperatureAsk();
    text.text = I18N_TELEGRAM_ENTER_TEMPERATURE_ON;
    break;
  case TELEGRAM_SETTINGS_ASK_TEMPERATURE_OFF:
    menu = getInlineMenuWithOffTemperatureAsk();
    text.text = I18N_TELEGRAM_ENTER_TEMPERATURE_OFF;
    break;
  case TELEGRAM_SETTINGS_MENU_HIDDEN:
    closeInlineMenu();
    return messageId;
  default:
    break;
  }
  if (bot == nullptr) {
    return messageId;
  }
  fb::MenuEdit menuEdit(messageId, chatId, menu);
  if (text.text) {
    text.messageID = messageId;
    text.chatID = chatId;
    bot->editText(text);
  }
  bot->editMenu(menuEdit);
  return messageId;
}

fb::InlineMenu TelegramManager::getInlineMenuWithTemperatureAsk() {
  fb::InlineMenu menu;
  menu.addButton(I18N_TELEGRAM_INLINE_MENU_ON_TEMPERATURE,
                 TELEGRAM_CHANGE_ON_BUTTON);
  menu.addButton(I18N_TELEGRAM_INLINE_MENU_OFF_TEMPERATURE,
                 TELEGRAM_CHANGE_OFF_BUTTON);
  menu.newRow();
  menu.addButton(I18N_TELEGRAM_INLINE_MENU_EXIT, TELEGRAM_EXIT_BUTTON);
  return menu;
}

fb::InlineMenu TelegramManager::getInlineMenuWithOffTemperatureAsk() {
  fb::InlineMenu menu;
  menu.addButton(I18N_TELEGRAM_INLINE_MENU_EXIT, TELEGRAM_EXIT_BUTTON);
  return menu;
}

fb::InlineMenu TelegramManager::getInlineMenuWithOnTemperatureAsk() {
  fb::InlineMenu menu;
  menu.addButton(I18N_TELEGRAM_INLINE_MENU_EXIT, TELEGRAM_EXIT_BUTTON);
  return menu;
}

void TelegramManager::closeInlineMenu() {
  if (bot == nullptr) {
    return;
  }

  int messageId = globalState.configuration.telegramSettingsMenu.messageId;
  Serial.print("Message to delete: ");
  Serial.println(messageId);
  fb::Result result = bot->deleteMessage(chatId, messageId);

  Serial.println(result.getRaw());
  globalState.configuration.telegramSettingsMenu.messageId = 0;
}
