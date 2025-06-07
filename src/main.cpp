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
      cfg.y_min = 200;
      cfg.y_max = 3900;
      cfg.bus_shared = true;
      cfg.offset_rotation = 3;
      cfg.pin_int = -1;
      _touch.config(cfg);
      _panel.setTouch(&_touch);
    }

    setPanel(&_panel);
  }
};

LGFX tft;
int screen_state = 0;
const int total_screens = 3;
lgfx::touch_point_t tp;
bool circleState[9] = {true,true,true,true,true,true,true,true,true};

void drawCircleGrid() {
  int radius = 20;
  int spacing = 60;
  int startX = 50;
  int startY = 60;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      int idx = i * 3 + j;
      int cx = startX + j * spacing;
      int cy = startY + i * spacing;
      tft.fillCircle(cx, cy, radius, circleState[idx] ? TFT_GREEN : TFT_RED);
    }
  }
}

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
  tft.setCursor(80, 20);
  tft.print("Screen 3");
  drawCircleGrid();
  tft.fillRect(200, 200, 100, 40, TFT_GREEN);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(230, 215);
  tft.print("Next");
}

void showCurrentScreen() {
  switch (screen_state) {
    case 0: showScreen0(); break;
    case 1: showScreen1(); break;
    case 2: showScreen2(); break;
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

    if (screen_state == 2) {
      // チェック9個の丸
      int radius = 20;
      int spacing = 60;
      int startX = 50;
      int startY = 60;
      for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
          int idx = i * 3 + j;
          int cx = startX + j * spacing;
          int cy = startY + i * spacing;
          int dx = tp.x - cx;
          int dy = tp.y - cy;
          if (dx * dx + dy * dy < radius * radius) {
            circleState[idx] = !circleState[idx];
            tft.fillCircle(cx, cy, radius, circleState[idx] ? TFT_GREEN : TFT_RED);
            delay(300);
            return;
          }
        }
      }
      if (tp.x > 200 && tp.x < 300 && tp.y > 200 && tp.y < 240) {
        screen_state = (screen_state + 1) % total_screens;
        showCurrentScreen();
        delay(300);
      }
    } else {
      if (tp.x > 100 && tp.x < 220 && tp.y > 200 && tp.y < 240) {
        screen_state = (screen_state + 1) % total_screens;
        showCurrentScreen();
        delay(300);
      }
    }
  }
}
