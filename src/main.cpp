#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
public:
  LGFX(void) {
    auto bus = new lgfx::Bus_SPI();
    {
      lgfx::Bus_SPI::config_t cfg;
      cfg.spi_host = VSPI_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 40000000;
      cfg.freq_read  = 16000000;
      cfg.spi_3wire  = false;
      cfg.use_lock   = true;
      cfg.dma_channel = 1;
      cfg.pin_sclk = 18;
      cfg.pin_mosi = 23;
      cfg.pin_miso = -1;
      cfg.pin_dc   = 21;
      bus->config(cfg);
    }
    this->setBus(bus);

    auto panel = new lgfx::Panel_ILI9341();
    {
      lgfx::Panel_ILI9341::config_t cfg;
      cfg.pin_cs   = 5;
      cfg.pin_rst  = 22;
      cfg.pin_busy = -1;
      cfg.memory_width  = 240;
      cfg.memory_height = 320;
      cfg.panel_width   = 240;
      cfg.panel_height  = 320;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits  = 1;
      cfg.readable = false;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;
      panel->config(cfg);
    }
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
