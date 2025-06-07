#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
public:
  lgfx::Panel_ST7796 _panel_instance;
  lgfx::Bus_SPI _bus_instance;

  LGFX(void) {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI2_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 80000000;
      cfg.freq_read  = 16000000;
      cfg.spi_3wire  = false;
      cfg.use_lock   = true;
      cfg.dma_channel = 1;
      cfg.pin_sclk = 14;
      cfg.pin_mosi = 13;
      cfg.pin_miso = 12;
      cfg.pin_dc   = 2;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs   = 15;
      cfg.pin_rst  = -1;
      cfg.pin_busy = -1;

      cfg.memory_width  = 320;
      cfg.memory_height = 480;
      cfg.panel_width   = 320;
      cfg.panel_height  = 480;

      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;
      _panel_instance.config(cfg);
    }

    setPanel(&_panel_instance);
  }
};

LGFX tft;

enum ScreenState {
  SCREEN_HOME,
  SCREEN_MENU
};

ScreenState currentScreen = SCREEN_HOME;

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

void handleTouchOrInput() {
  if (digitalRead(0) == LOW) {
    delay(200);
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
  tft.init();
  tft.setRotation(1);
  tft.setBrightness(255);
  drawHomeScreen();
}

void loop() {
  handleTouchOrInput();
}
