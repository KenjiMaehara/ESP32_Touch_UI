#pragma once
#include <LovyanGFX.hpp>

class MyLGFX : public lgfx::LGFX_Device {
  // SPIバス設定
  lgfx::Bus_SPI _bus_instance;

  // パネルとタッチをnewで動的生成（v0.4.7仕様に適合）
  lgfx::Panel_ILI9488* _panel_instance = nullptr;
  lgfx::Touch_XPT2046* _touch_instance = nullptr;

public:
  LGFX() {
    _panel_instance = new lgfx::Panel_ILI9488();
    _touch_instance = new lgfx::Touch_XPT2046();

    { // SPIバス設定
      auto cfg = _bus_instance.config();
      cfg.spi_host = VSPI_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 40000000;
      cfg.freq_read  = 16000000;
      cfg.spi_3wire = false;
      cfg.use_lock = true;
      cfg.dma_channel = 1;
      cfg.pin_sclk = 18;
      cfg.pin_mosi = 23;
      cfg.pin_miso = 19;
      cfg.pin_dc   = 2;
      cfg.pin_cs   = 15;
      cfg.pin_rst  = 4;
      cfg.pin_busy = -1;
      _bus_instance.config(cfg);
      _panel_instance->setBus(&_bus_instance);
    }

    { // パネル設定
      auto cfg = _panel_instance->config();
      cfg.panel_width  = 320;
      cfg.panel_height = 480;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits  = 1;
      cfg.readable = true;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;
      _panel_instance->config(cfg);
    }

    { // タッチ設定
      auto cfg = _touch_instance->config();
      cfg.x_min = 200;
      cfg.x_max = 3900;
      cfg.y_min = 200;
      cfg.y_max = 3900;
      cfg.pin_int  = -1;
      cfg.bus_shared = true;
      cfg.offset_rotation = 0;
      cfg.spi_host = VSPI_HOST;
      cfg.freq = 1000000;
      cfg.pin_sclk = 18;
      cfg.pin_mosi = 23;
      cfg.pin_miso = 19;
      cfg.pin_cs   = 33;
      _touch_instance->config(cfg);
      _panel_instance->setTouch(_touch_instance);
    }

    setPanel(_panel_instance);  // ここが v0.4.7 向けに正しい呼び出し
  }
};
