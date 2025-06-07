#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
public:
  lgfx::Panel_ILI9488_8080 _panel;
  lgfx::Bus_Parallel8 _bus;

  LGFX(void) {
    {
      auto cfg = _bus.config();
      cfg.port = 0;
      cfg.freq_write = 20000000;
      cfg.pin_wr = 4;     // WRX
      cfg.pin_rd = -1;    // RD (未使用)
      cfg.pin_rs = 2;     // DCX

      // データバス（DB0～DB7）
      cfg.pin_d0 = 32;
      cfg.pin_d1 = 33;
      cfg.pin_d2 = 25;
      cfg.pin_d3 = 26;
      cfg.pin_d4 = 27;
      cfg.pin_d5 = 14;
      cfg.pin_d6 = 12;
      cfg.pin_d7 = 13;

      _bus.config(cfg);
      _panel.setBus(&_bus);
    }

    {
      auto cfg = _panel.config();
      cfg.pin_cs = 15;
      cfg.pin_rst = -1;
      cfg.pin_busy = -1;
      cfg.memory_width = 320;
      cfg.memory_height = 480;
      cfg.panel_width = 320;
      cfg.panel_height = 480;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;
      _panel.config(cfg);
    }

    _panel.setLight(nullptr); // バックライト常時ON
    setPanel(&_panel);
  }
};

LGFX tft;

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.setCursor(80, 120);
  tft.print("Hello, World!");
}

void loop() {
  // 何もしない
}
