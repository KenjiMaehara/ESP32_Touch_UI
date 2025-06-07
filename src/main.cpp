#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7796 _panel;
  lgfx::Bus_SPI _bus;

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
      cfg.dma_channel = -1;  // 無効化して安全に
      cfg.pin_sclk = 14;  // LCD_SCK
      cfg.pin_mosi = 13;  // LCD_MOSI
      cfg.pin_miso = -1;  // LCD_MISO not used
      cfg.pin_dc   = 2;   // LCD_RS (D/C)
      _bus.config(cfg);
      _panel.setBus(&_bus);
    }

    {
      auto cfg = _panel.config();
      cfg.pin_cs   = 15;   // LCD_CS
      cfg.pin_rst  = -1;   // LCD_RST (ENと共有、未制御)
      cfg.pin_busy = -1;
      cfg.memory_width  = 320;
      cfg.memory_height = 480;
      cfg.panel_width   = 320;
      cfg.panel_height  = 480;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;
      _panel.config(cfg);
    }

    _panel.setLight(nullptr);  // バックライト処理を一時的にバイパス
    setPanel(&_panel);
  }
};

LGFX tft;

void setup() {
  Serial.begin(115200);
  Serial.println("Start setup");

  tft.init();
  Serial.println("tft.init OK");

  tft.setRotation(1);
  Serial.println("setRotation OK");

  tft.fillScreen(TFT_BLACK);
  Serial.println("fillScreen OK");

  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.setCursor(80, 120);
  tft.print("Hello, World!");
  Serial.println("print OK");
}

void loop() {
  // 何もしない
}
