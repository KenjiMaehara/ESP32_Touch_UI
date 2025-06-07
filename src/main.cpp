#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
public:
  LGFX() {
    auto bus = new lgfx::Bus_SPI();
    bus->config.spi_host = VSPI_HOST;
    bus->config.spi_mode = 0;
    bus->config.freq_write = 40000000;
    bus->config.pin_sclk = 18;
    bus->config.pin_mosi = 23;
    bus->config.pin_miso = -1;
    bus->config.pin_dc   = 21;
    this->setBus(bus);

    auto panel = new lgfx::Panel_ILI9341();
    panel->config.pin_cs   = 5;
    panel->config.pin_rst  = 22;
    panel->config.pin_busy = -1;
    this->setPanel(panel);
  }
};

LGFX tft;

enum ScreenState {
  SCREEN_HOME,
  SCREEN_MENU
};

ScreenState currentScreen = SCREEN_HOME;

// 画面描画関数
void drawHomeScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("HOME Screen");

  tft.drawRect(50, 100, 120, 40, TFT_WHITE);
  tft.setCursor(60, 115);
  tft.print("Go to MENU");
}

void drawMenuScreen() {
  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("MENU Screen");

  tft.drawRect(50, 100, 120, 40, TFT_YELLOW);
  tft.setCursor(60, 115);
  tft.print("Back to HOME");
}

// タッチ処理（仮：タッチなしの場合はボタン押下などで代用）
void handleTouchOrInput() {
  // ここでは仮にボタン（GPIO 0）で画面遷移する例
  if (digitalRead(0) == LOW) {
    delay(200);  // デバウンス
    if (currentScreen == SCREEN_HOME) {
      currentScreen = SCREEN_MENU;
      drawMenuScreen();
    } else {
      currentScreen = SCREEN_HOME;
      drawHomeScreen();
    }
  }
}

void setup() {
  pinMode(0, INPUT_PULLUP);
  tft.begin();
  drawHomeScreen();
}

void loop() {
  handleTouchOrInput();
}
