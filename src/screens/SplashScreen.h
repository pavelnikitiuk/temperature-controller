// #ifndef SCREENS_H
// #define SCREENS_H

// #include <U8g2lib.h>

// class MainScreen : public U8g2Menu {
//   public:
//     MainScreen(U8G2 &u8g2) : U8g2Menu(u8g2) {}
    
//     void draw() override {
//       u8g2.setFont(u8g2_font_6x10_tf);
//       u8g2.drawStr(0, 10, "Main Screen");
//       u8g2.drawStr(0, 25, "IP: 192.168.1.100");
//       u8g2.drawStr(0, 40, "Status: Online");
//     }
// };

// class SettingsScreen : public U8g2Menu {
//   public:
//     SettingsScreen(U8G2 &u8g2) : U8g2Menu(u8g2) {}
    
//     void draw() override {
//       u8g2.setFont(u8g2_font_6x10_tf);
//       u8g2.drawStr(0, 10, "Settings");
//       u8g2.drawStr(0, 25, "WiFi: MySSID");
//       u8g2.drawStr(0, 40, "Update: Every 24h");
//     }
// };

// #endif