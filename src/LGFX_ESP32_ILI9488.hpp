#pragma once
#include <LovyanGFX.hpp>

// カスタム設定用の構成クラス
struct LGFX_Config_ILI9488 : public lgfx::LGFX_Config {
  LGFX_Config_ILI9488() {
    spi_host = VSPI_HOST;
    spi_mode = 0;
    freq = 40000000;

    pin_cs   = 15;
    pin_rst  = 4;
    pin_dc   = 2;
    pin_sclk = 18;
    pin_mosi = 23;
    pin_miso = 19;

    panel_width  = 320;
    panel_height = 480;
    offset_rotation = 0;
    invert     = false;
    rgb_order  = false;
    bus_shared = true;

    pin_touch_cs = 33;
    touch_freq = 1000000;
    x_min = 200;
    x_max = 3900;
    y_min = 200;
    y_max = 3900;
    offset_touch_rotation = 0;
  }
};

// LGFX 本体クラス (v0.4.x 用)
class MyLGFX : public lgfx::LGFX_SPI<LGFX_Config_ILI9488> {};
