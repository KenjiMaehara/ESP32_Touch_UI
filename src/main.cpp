#include <lvgl.h>
#include <LovyanGFX.hpp>
#include <time.h>
#include "clock_page.h"
#include "color_button_page.h"

static const uint32_t screenWidth  = 480;
static const uint32_t screenHeight = 320;
static lv_color_t* buf1 = nullptr;
static lv_color_t* buf2 = nullptr;
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;
static lv_disp_t* disp;
static lv_indev_t* indev;

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7796  _panel_instance;
  lgfx::Bus_SPI       _bus_instance;
  lgfx::Light_PWM     _light_instance;
  lgfx::Touch_XPT2046 _touch_instance;

public: LGFX(void) {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI2_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 80000000;
      cfg.freq_read = 16000000;
      cfg.pin_sclk = 14;
      cfg.pin_mosi = 13;
      cfg.pin_miso = 12;
      cfg.pin_dc = 2;
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }
    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs = 15;
      cfg.pin_rst = -1;
      cfg.pin_busy = -1;
      cfg.memory_width = screenHeight;
      cfg.memory_height = screenWidth;
      cfg.panel_width = screenHeight;
      cfg.panel_height = screenWidth;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = false;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;
      _panel_instance.config(cfg);
    }
    {
      auto cfg = _light_instance.config();
      cfg.pin_bl = 27;
      cfg.invert = false;
      cfg.freq = 44100;
      cfg.pwm_channel = 7;
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }
    {
      auto cfg = _touch_instance.config();
      cfg.x_min = 0;
      cfg.x_max = 4095;
      cfg.y_min = 4095;
      cfg.y_max = 0;
      cfg.offset_rotation = 0;
      cfg.spi_host = SPI2_HOST;
      cfg.freq = 1000000;
      cfg.pin_sclk = 14;
      cfg.pin_mosi = 13;
      cfg.pin_miso = 12;
      cfg.pin_cs = 33;
      cfg.pin_int = -1;
      cfg.bus_shared = true;
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }
    setPanel(&_panel_instance);
  }
};

LGFX tft;

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
  tft.pushPixels((uint16_t *)color_p, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
  tft.endWrite();
  lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  static bool pressed = false;
  static lv_point_t last_point;
  int x, y;
  if (tft.getTouch(&x, &y)) {
    last_point.x = x;
    last_point.y = y;
    pressed = true;
  } else {
    pressed = false;
  }
  data->point = last_point;
  data->state = pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  lv_init();

  configTime(9 * 3600, 0, "ntp.nict.jp");
  setenv("TZ", "JST-9", 1);
  tzset();

  buf1 = (lv_color_t*)heap_caps_malloc(screenWidth * 40 * sizeof(lv_color_t), MALLOC_CAP_DMA);
  buf2 = (lv_color_t*)heap_caps_malloc(screenWidth * 40 * sizeof(lv_color_t), MALLOC_CAP_DMA);
  if (!buf1 || !buf2) while (1);
  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screenWidth * 40);

  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp = lv_disp_drv_register(&disp_drv);

  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  indev = lv_indev_drv_register(&indev_drv);

  create_clock_screen();
}

unsigned long last_tick = 0;
unsigned long last_second_update = 0;

void loop() {
  unsigned long now = millis();
  if (now - last_tick > 5) {
    lv_tick_inc(now - last_tick);
    last_tick = now;
  }
  lv_timer_handler();

  // clock_label が有効かどうかだけでなく、オブジェクトの親が NULL でないことも確認
  if (clock_label && lv_obj_get_parent(clock_label)) {
    if (millis() - last_second_update >= 1000) {
      last_second_update = millis();
      time_t now_time = time(NULL);
      struct tm *tm_info = localtime(&now_time);
      char time_str[6];
      strftime(time_str, sizeof(time_str), "%H:%M", tm_info);
      lv_label_set_text(clock_label, time_str);
    }
  }

  delay(1);
}
