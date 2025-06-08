#include <Arduino.h>
#include <LovyanGFX.hpp>

// --- MyButton クラス（LGFX_Button の競合回避版） ---
class MyButton {
public:
  int16_t x, y, w, h;
  uint16_t outlinecolor, fillcolor, textcolor;
  const char* label;
  uint8_t textsize;
  LGFX* gfx;

  void initButton(LGFX* gfx_, int16_t x_, int16_t y_, int16_t w_, int16_t h_, uint16_t outline, uint16_t fill, uint16_t text, const char* label_, uint8_t textsize_) {
    gfx = gfx_;
    x = x_ - w_ / 2;
    y = y_ - h_ / 2;
    w = w_;
    h = h_;
    outlinecolor = outline;
    fillcolor = fill;
    textcolor = text;
    label = label_;
    textsize = textsize_;
  }

  void drawButton(bool inverted = false) {
    uint16_t fill = inverted ? textcolor : fillcolor;
    uint16_t text = inverted ? fillcolor : textcolor;
    gfx->fillRect(x, y, w, h, fill);
    gfx->drawRect(x, y, w, h, outlinecolor);
    int16_t x_c = x + (w / 2) - (strlen(label) * 6 * textsize / 2);
    int16_t y_c = y + (h / 2) - (8 * textsize / 2);
    gfx->setTextColor(text);
    gfx->setTextSize(textsize);
    gfx->setCursor(x_c, y_c);
    gfx->print(label);
  }

  bool contains(int16_t tx, int16_t ty) {
    return (tx >= x && tx < (x + w) && ty >= y && ty < (y + h));
  }
};

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
MyButton nextButton;

static const char* keys[4][3] = {
  {"1", "2", "3"},
  {"4", "5", "6"},
  {"7", "8", "9"},
  {"*", "0", "#"}
};

void drawNextButton(int x, int y, int w, int h) {
  nextButton.initButton(&tft, x + w/2, y + h/2, w, h, TFT_WHITE, TFT_GREEN, TFT_BLACK, "Next", 2);
  nextButton.drawButton();
}

void showScreen0() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(3);
  tft.setCursor(80, 40);
  tft.print("Screen 1");
  drawNextButton(100, 200, 120, 40);
}

void showScreen1() {
  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(3);
  tft.setCursor(80, 40);
  tft.print("Screen 2");
  drawNextButton(100, 200, 120, 40);
}

void showScreen2() {
  tft.fillScreen(TFT_PURPLE);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(80, 40);
  tft.print("Screen 3");
  drawNextButton(100, 200, 120, 40);
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

  drawNextButton(240, 260, 60, 40);
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

    if (screen_state < 3 && nextButton.contains(tp.x, tp.y)) {
      screen_state = (screen_state + 1) % total_screens;
      showCurrentScreen();
      delay(300);
    }
    else if (screen_state == 3) {
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
      if (nextButton.contains(tp.x, tp.y)) {
        screen_state = 0;
        showCurrentScreen();
        delay(300);
        return;
      }
    }
  }
}
