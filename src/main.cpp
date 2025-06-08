#include <Arduino.h>
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7796 _panel;
  lgfx::Bus_SPI _bus;
  lgfx::Light_PWM _light;
  lgfx::Touch_XPT2046 _touch;

public:
  LGFX(void) {
    {
      auto cfg = _bus.config();
      cfg.spi_host = VSPI_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 80000000;
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
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = true;
      _panel.config(cfg);
    }

    {
      auto cfg = _light.config();
      cfg.pin_bl = 27;
      cfg.invert = false;
      cfg.freq = 44100;
      cfg.pwm_channel = 7;
      _light.config(cfg);
      _panel.setLight(&_light);
    }

    {
      auto cfg = _touch.config();
      cfg.spi_host = VSPI_HOST;
      cfg.freq = 1000000;
      cfg.pin_sclk = 14;
      cfg.pin_mosi = 13;
      cfg.pin_miso = 12;
      cfg.pin_cs   = 33;
      cfg.x_min = 200;
      cfg.x_max = 3900;
      cfg.y_min = 3900;
      cfg.y_max = 200;
      cfg.bus_shared = true;
      cfg.offset_rotation = 0;
      cfg.pin_int = -1;
      _touch.config(cfg);
      _panel.setTouch(&_touch);
    }

    setPanel(&_panel);
  }
};

LGFX tft;
int screen_state = 0;
const int total_screens = 4;
lgfx::touch_point_t tp;
String input_value = "";

static const char* keys[4][3] = {
  {"1", "2", "3"},
  {"4", "5", "6"},
  {"7", "8", "9"},
  {"*", "0", "#"}
};

void showScreen0() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(3);
  tft.setCursor(80, 40);
  tft.print("Screen 1");
  tft.fillRect(100, 200, 120, 40, TFT_GREEN);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(135, 215);
  tft.print("Next");
}

void showScreen1() {
  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(3);
  tft.setCursor(80, 40);
  tft.print("Screen 2");
  tft.fillRect(100, 200, 120, 40, TFT_ORANGE);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(135, 215);
  tft.print("Next");
}

void showScreen2() {
  tft.fillScreen(TFT_PURPLE);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(80, 40);
  tft.print("Screen 3");
  tft.fillRect(100, 200, 120, 40, TFT_CYAN);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(135, 215);
  tft.print("Next");
}

void showScreen3() {
  tft.fillScreen(TFT_DARKGREY);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(40, 20);
  tft.print("Input: " + input_value);

  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 3; col++) {
      int x = 40 + col * 80;
      int y = 40 + row * 70;
      tft.fillRect(x, y, 60, 60, TFT_WHITE);
      tft.setTextColor(TFT_BLACK);
      tft.setTextSize(3);
      tft.setCursor(x + 20, y + 15);
      tft.print(keys[row][col]);
    }
  }

  // Nextボタン
  tft.fillRect(100, 340, 120, 40, TFT_GREEN);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(135, 355);
  tft.print("Next");
}

void showCurrentScreen() {
  switch (screen_state) {
    case 0: showScreen0(); break;
    case 1: showScreen1(); break;
    case 2: showScreen2(); break;
    case 3: showScreen3(); break;
  }
}

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setBrightness(255);
  tft.setRotation(1);
  showCurrentScreen();
}

void loop() {
  if (tft.getTouch(&tp)) {
    Serial.printf("Touch: x=%d y=%d\n", tp.x, tp.y);

    if (tp.x > 100 && tp.x < 220 && tp.y > 200 && tp.y < 240 && screen_state < 3) {
      screen_state = (screen_state + 1) % total_screens;
      showCurrentScreen();
      delay(300);
    }
    else if (screen_state == 3) {
      // テンキータッチ
      for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 3; col++) {
          int x = 40 + col * 80;
          int y = 40 + row * 70;
          if (tp.x > x && tp.x < x + 60 && tp.y > y && tp.y < y + 60) {
            input_value += keys[row][col];
            showScreen3();
            delay(300);
            return;
          }
        }
      }
      // Nextボタン判定
      if (tp.x > 100 && tp.x < 220 && tp.y > 340 && tp.y < 380) {
        screen_state = 0;
        showCurrentScreen();
        delay(300);
        return;
      }
    }
  }
}
