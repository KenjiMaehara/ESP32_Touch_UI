#include <lvgl.h>
#include <LovyanGFX.hpp>

static const uint32_t screenWidth  = 480;
static const uint32_t screenHeight = 320;
static const uint32_t draw_buf_lines = 40;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * draw_buf_lines];  // シングルバッファに変更

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
      cfg.x_min = 222;
      cfg.x_max = 3367;
      cfg.y_min = 192;
      cfg.y_max = 3732;
      cfg.pin_int = -1;
      cfg.bus_shared = true;
      cfg.offset_rotation = 0;
      cfg.spi_host = SPI2_HOST;
      cfg.freq = 1000000;
      cfg.pin_sclk = 14;
      cfg.pin_mosi = 13;
      cfg.pin_miso = 12;
      cfg.pin_cs = 33;
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
  if (tft.getTouch(&data->point.x, &data->point.y)) {
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

void btn_event_cb(lv_event_t *e) {
  lv_obj_t *label = (lv_obj_t *)lv_event_get_user_data(e);
  lv_label_set_text(label, "PRESSED!");
}

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);

  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * draw_buf_lines);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.sw_rotate = 0;                 // ← 無効化
  disp_drv.rotated = LV_DISP_ROT_NONE;    // ← 無効化
  lv_disp_drv_register(&disp_drv);

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  lv_obj_t *btn = lv_btn_create(lv_scr_act());
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
  lv_obj_t *label = lv_label_create(btn);
  lv_label_set_text(label, "Click me!");
  lv_obj_center(label);
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, label);

  lv_obj_t* bg_rect = lv_obj_create(lv_scr_act());
  lv_obj_set_style_border_width(bg_rect, 4, 0);
  lv_obj_set_style_border_color(bg_rect, lv_color_hex(0xFF0000), 0);

  lv_obj_set_size(bg_rect, screenWidth, screenHeight);  // ← 480x320に戻す

  lv_obj_align(bg_rect, LV_ALIGN_TOP_LEFT, 0, 0);
}

void loop() {
  lv_timer_handler();
  delay(5);
}
