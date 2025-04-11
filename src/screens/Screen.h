// #ifndef SCREEN_H
// #define SCREEN_H

// #include <U8g2lib.h>

// // Базовый класс экрана [[4]]
// class Screen {
//   public:
//     virtual void draw(U8G2 &u8g2) = 0;
//     virtual void updateData(const void *data) = 0; // Универсальный метод обновления данных
//     virtual ~Screen() = default;
// };

// // Менеджер экранов
// class ScreenManager {
//   private:
//     Screen *currentScreen;
//     U8G2 &u8g2;

//   public:
//     ScreenManager(U8G2 &display) : u8g2(display), currentScreen(nullptr) {}

//     void setScreen(Screen &screen) {
//       currentScreen = &screen;
//     }

//     void update() {
//       if (currentScreen) {
//         currentScreen->draw(u8g2);
//       }
//     }
// };

// #endif

// // Реализация экранов с разными данными
// struct MainScreenData {
//   float temperature;
//   bool isRelayActive;
// };

// class MainScreen : public Screen {
//   private:
//     MainScreenData data;

//   public:
//     void updateData(const void *newData) override {
//       if (newData) {
//         data = *static_cast<const MainScreenData*>(newData);
//       }
//     }

//     void draw(U8G2 &u8g2) override {
//       u8g2.clearBuffer();
//       u8g2.setFont(u8g2_font_ncenB08_tr);
//       u8g2.drawStr(0, 10, "Main Screen");
//       u8g2.setCursor(0, 30);
//       u8g2.print(data.temperature);
//       u8g2.sendBuffer();
//     }
// };

// struct SettingsScreenData {
//   int brightness;
//   String mode;
// };

// class SettingsScreen : public Screen {
//   private:
//     SettingsScreenData data;

//   public:
//     void updateData(const void *newData) override {
//       if (newData) {
//         data = *static_cast<const SettingsScreenData*>(newData);
//       }
//     }

//     void draw(U8G2 &u8g2) override {
//       u8g2.clearBuffer();
//       u8g2.setFont(u8g2_font_ncenB08_tr);
//       u8g2.drawStr(0, 10, "Settings");
//       u8g2.setCursor(0, 30);
//       u8g2.print(data.brightness);
//       u8g2.sendBuffer();
//     }
// };

// // Пример использования
// U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, 12, 11, -1);
// ScreenManager screenManager(u8g2);
// MainScreen mainScreen;
// SettingsScreen settingsScreen;

// void setup() {
//   u8g2.begin();
  
//   // Устанавливаем начальный экран
//   screenManager.setScreen(mainScreen);
  
//   // Пример обновления данных
//   MainScreenData mainData = {25.5, true};
//   mainScreen.updateData(&mainData);
  
//   SettingsScreenData settingsData = {80, "Auto"};
//   settingsScreen.updateData(&settingsData);
// }

// void loop() {
//   screenManager.update();
  
//   // Пример переключения экранов
//   static bool toggle = false;
//     toggle = !toggle;
//     screenManager.setScreen(toggle ? mainScreen : settingsScreen);
// }