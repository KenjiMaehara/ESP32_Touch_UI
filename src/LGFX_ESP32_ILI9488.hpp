#pragma once
#include <LovyanGFX.hpp>

class MyLGFX : public lgfx::LGFX_Device {
  lgfx::v0::Panel_ILI9488* _panel;
  lgfx::v0::Bus_SPI* _bus;
  lgfx::v0::Touch_XPT2046* _touch;

public:
  MyLGFX() {
    _panel = new lgfx::v0::Panel_ILI9488();
    _bus = new lgfx::v0::Bus_SPI();
    _touch = new lgfx::v0::Touch_XPT2046();

    { // SPIバス設定
      auto cfg = _bus->config();
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
      _bus->config(cfg);
      _panel->setBus(_bus);
    }

    { // パネル設定
      auto cfg = _panel->config();
      cfg.panel_width = 320;
      cfg.panel_height = 480;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits  = 1;
      cfg.readable = true;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;
      _panel->config(cfg);
    }

    { // タッチパネル設定
      auto cfg = _touch->config();
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
      _touch->config(cfg);
      _panel->setTouch(_touch);
    }

    setPanel(_panel);  // これで純粋仮想関数が満たされる
  }
};
