#include <lvgl.h>
#include <LovyanGFX.hpp>
#include "esp_heap_caps.h"

static const uint32_t screenWidth  = 480;
static const uint32_t screenHeight = 320;
static lv_color_t buf1[screenWidth * 40];
static lv_color_t* buf2 = nullptr;
static lv_disp_draw_buf_t draw_buf;

lv_obj_t *label;

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
      cfg.spi_3wire = false;
      cfg.use_lock = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
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
      cfg.x_min = 250;      // 実測に基づく補正
      cfg.x_max = 3900;
      cfg.y_min = 300;
      cfg.y_max = 3700;
      cfg.offset_rotation = 1;
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
  tft.pushColors((uint16_t *)color_p, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
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
    Serial.printf("Touch: x=%d, y=%d\n", x, y);  // デバッグ表示
  } else {
    pressed = false;
  }

  data->point = last_point;
  data->state = pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

void btn_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  Serial.printf("[Event] code = %d\n", code);

  if (code == LV_EVENT_CLICKED) {
    lv_label_set_text(label, "PRESSED!");
    Serial.println("Button CLICKED!");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.printf("LV_HOR_RES_MAX = %d\n", LV_HOR_RES_MAX);
  Serial.printf("LV_VER_RES_MAX = %d\n", LV_VER_RES_MAX);

  tft.init();
  tft.setRotation(1);  // offset_rotation =_
}