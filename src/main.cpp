#include <LovyanGFX.hpp>
#include <vector>

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7796 _panel;
  lgfx::Bus_SPI _bus;
  lgfx::Touch_XPT2046 _touch;

public:
  LGFX(void) {
    {
      auto cfg = _bus.config();
      cfg.spi_host = VSPI_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 40000000;
      cfg.freq_read = 16000000;
      cfg.spi_3wire = false;
      cfg.use_lock = true;
      cfg.dma_channel = -1;
      cfg.pin_sclk = 14;
      cfg.pin_mosi = 13;
      cfg.pin_miso = -1;
      cfg.pin_dc   = 2;
      _bus.config(cfg);
      _panel.setBus(&_bus);
    }

    {
      auto cfg = _panel.config();
      cfg.pin_cs   = 15;
      cfg.pin_rst  = -1;
      cfg.pin_busy = -1;
      cfg.memory_width  = 320;
      cfg.memory_height = 480;
      cfg.panel_width   = 320;
      cfg.panel_height  = 480;
      cfg.offset_rotation = 3;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;
      _panel.config(cfg);
    }

    {
      auto cfg = _touch.config();
      cfg.spi_host = VSPI_HOST;
      cfg.freq = 1000000;
      cfg.pin_sclk = 25;
      cfg.pin_mosi = 32;
      cfg.pin_miso = 39;
      cfg.pin_cs   = 33;
      cfg.x_min = 800;
      cfg.x_max = 3795;
      cfg.y_min = 100;
      cfg.y_max = 3944;
      cfg.bus_shared = true;
      _touch.config(cfg);
      _panel.setTouch(&_touch);
    }

    _panel.setLight(nullptr);
    setPanel(&_panel);
  }
};

LGFX tft;

int currentScreen = 0;

void drawScreen0() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.setCursor(80, 120);
  tft.print("Hello, World!");

  tft.fillRect(220, 180, 80, 40, TFT_GREEN);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(235, 195);
  tft.print("Next");
}

void drawScreen1() {
  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(60, 100);
  tft.print("This is Screen 2");

  tft.fillRect(220, 180, 80, 40, TFT_GREEN);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(235, 195);
  tft.print("Next");
}

void drawScreen2() {
  tft.fillScreen(TFT_YELLOW);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(60, 100);
  tft.print("Touch the dots");

  for (int i = 0; i < 9; ++i) {
    int x = 40 + (i % 3) * 80;
    int y = 150 + (i / 3) * 80;
    tft.fillCircle(x, y, 20, TFT_GREEN);
  }

  tft.fillRect(220, 10, 80, 40, TFT_RED);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(235, 25);
  tft.print("Next");
}

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  drawScreen0();
}

void loop() {
  uint16_t x, y;
  if (tft.getTouch(&x, &y)) {
    Serial.printf("Touch: x=%d y=%d\n", x, y);
    if (currentScreen == 0 && x > 220 && x < 300 && y > 180 && y < 220) {
      currentScreen = 1;
      drawScreen1();
    } else if (currentScreen == 1 && x > 220 && x < 300 && y > 180 && y < 220) {
      currentScreen = 2;
      drawScreen2();
    } else if (currentScreen == 2 && x > 220 && x < 300 && y > 10 && y < 50) {
      currentScreen = 0;
      drawScreen0();
    } else if (currentScreen == 2) {
      for (int i = 0; i < 9; ++i) {
        int cx = 40 + (i % 3) * 80;
        int cy = 150 + (i / 3) * 80;
        if ((x - cx) * (x - cx) + (y - cy) * (y - cy) < 400) {
          tft.fillCircle(cx, cy, 20, TFT_RED);
        }
      }
    }
    delay(300);
  }
}
