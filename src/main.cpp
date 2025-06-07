#include <Arduino.h>
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7796 _panel;
  lgfx::Bus_SPI _bus;
  lgfx::Light_PWM _light;

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

    setPanel(&_panel);
  }
};

LGFX tft;
int screen_state = 0;
unsigned long last_change = 0;
const unsigned long interval = 3000;  // 3秒ごとに切り替え

void showScreen0() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(3);
  tft.setCursor(40, 200);
  tft.print("Hello, World!");
}

void showScreen1() {
  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(3);
  tft.setCursor(20, 200);
  tft.print("Second Page");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start setup");

  tft.init();
  Serial.println("tft.init OK");

  tft.setBrightness(255);
  Serial.println("Brightness OK");

  tft.setRotation(1);
  Serial.println("setRotation OK");

  showScreen0();
  last_change = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - last_change > interval) {
    screen_state = !screen_state;
    if (screen_state == 0) showScreen0();
    else showScreen1();
    last_change = now;
  }
}