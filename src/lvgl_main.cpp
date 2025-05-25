#include "lvgl_main.h"
#include <lv_conf.h>
#include <lvgl.h>
#include <LovyanGFX.hpp>
#include "lgfx_jp_font_16.c"

// LovyanGFX v0.4.7 で自動定義されている LGFX クラスを利用
static LGFX lcd;

// LVGLバッファ
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

// LVGL → LovyanGFX 描画連携
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  lcd.startWrite();
  lcd.pushImage(area->x1, area->y1,
                area->x2 - area->x1 + 1,
                area->y2 - area->y1 + 1,
                (lgfx::rgb565_t*)&color_p[0]);
  lcd.endWrite();
  lv_disp_flush_ready(disp);
}

// 表示用テキスト
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

// ページ送り
void next_page_cb(lv_event_t* e) {
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

  // ラベル（本文）
  label = lv_label_create(lv_scr_act());
  lv_obj_set_width(label, lcd.width() - 40);
  lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 20);
  lv_obj_set_style_text_font(label, &lgfx_jp_font_16, LV_PART_MAIN);
  lv_label_set_text(label, story_pages[current_page]);
  lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);

  // 「つぎへ」ボタン
  lv_obj_t* btn = lv_btn_create(lv_scr_act());
  lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -20);
  lv_obj_set_size(btn, 120, 50);
  lv_obj_add_event_cb(btn, next_page_cb, LV_EVENT_CLICKED, NULL);

  lv_obj_t* btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "つぎへ ▶");
  lv_obj_center(btn_label);
}
