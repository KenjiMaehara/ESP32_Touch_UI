#include <Arduino.h>
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
      cfg.dma_channel = -1;
      cfg.pin_sclk = 14;
      cfg.pin_mosi = 13;
      cfg.pin_miso = -1;
      cfg.pin_dc   = 21;  // GPIO2から安全なGPIO21に変更
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
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;
      _panel.config(cfg);
    }

    _panel.setLight(nullptr);
    setPanel(&_panel);
  }
};

LGFX tft;

void setup() {
  Serial.begin(115200);
  Serial.println("Start setup");

  pinMode(27, OUTPUT);     // バックライト用ピン
  delay(10);
  digitalWrite(27, HIGH);  // バックライトON
  Serial.println("Backlight ON");

  tft.init();
  Serial.println("tft.init OK");

  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.setCursor(80, 120);
  tft.print("Hello, World!");
  Serial.println("print OK");
}

void loop() {
  // 何もしない
}
