#include "lvgl_main.h"
#include <lvgl.h>
#include <LovyanGFX.hpp>
#include "lgfx_jp_font_16.c"

// パネル定義（M5Core2 用）
class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_M5StackCore2 _panel;
  lgfx::Bus_SPI _bus;

public:
  LGFX(void) {
    {
      auto cfg = _bus.config();
      cfg.spi_host = VSPI_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 40000000;
      cfg.freq_read  = 16000000;
      cfg.spi_3wire = true;
      cfg.use_lock = true;
      cfg.dma_channel = 1;
      cfg.pin_sclk = 18;
      cfg.pin_mosi = 23;
      cfg.pin_miso = 19;
      cfg.pin_dc   = 27;
      _bus.config(cfg);
      _panel.setBus(&_bus);
    }

    {
      auto cfg = _panel.config();
      cfg.pin_cs   = 14;
      cfg.pin_rst  = 33;
      cfg.pin_busy = -1;
      _panel.config(cfg);
    }

    setPanel(&_panel);
  }
};

static LGFX lcd;

// バッファ設定
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

// LVGLとLovyanGFXの連携
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  lcd.startWrite();
  lcd.pushImage(area->x1, area->y1, area->x2 - area->x1 + 1,
                area->y2 - area->y1 + 1, (lgfx::rgb565_t *)&color_p[0]);
  lcd.endWrite();
  lv_disp_flush_ready(disp);
}

// ストーリーテキスト（表示テスト用）
const char* story_pages[] = {
  "むかしむかし、あるところに\nおじいさんとおばあさんが\n住んでいました。",
  "おじいさんは山へしばかりに、\nおばあさんは川へせんたくに\n行きました。",
  "川上から大きなももが\nどんぶらこ、どんぶらこと\n流れてきました。",
  "ももを家に持ち帰ると、\n中から元気な男の子が\n飛び出してきました。",
  "「桃から生まれた桃太郎です！」\nとその子は言いました。",
  "つづく…"
};

const int STORY_PAGE_COUNT = sizeof(story_pages) / sizeof(story_pages[0]);
int current_page = 0;
lv_obj_t* label;

// ページ切り替え
void next_page_cb(lv_event_t *e) {
  current_page = (current_page + 1) % STORY_PAGE_COUNT;
  lv_label_set_text(label, story_pages[current_page]);
}

void lvgl_setup() {
  lcd.init();
  lcd.setRotation(1);

  lv_init();

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, LV_HOR_RES_MAX * 10);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.hor_res = lcd.width();
  disp_drv.ver_res = lcd.height();
  lv_disp_drv_register(&disp_drv);

  // ラベル作成
  label = lv_label_create(lv_scr_act());
  lv_obj_set_width(label, lcd.width() - 40);
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);
  lv_obj_set_style_text_font(label, &lgfx_jp_font_16, LV_PART_MAIN);
  lv_label_set_text(label, story_pages[current_page]);
  lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);

  // 「つぎへ」ボタン
  lv_obj_t *btn = lv_btn_create(lv_scr_act());
  lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -20);
  lv_obj_set_size(btn, 120, 50);
  lv_obj_add_event_cb(btn, next_page_cb, LV_EVENT_CLICKED, NULL);

  lv_obj_t *btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "つぎへ ▶");
  lv_obj_center(btn_label);
}
