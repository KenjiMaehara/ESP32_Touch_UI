#include <lvgl.h>
#include "clock_page.h"
//#include "lv_font_montserrat_48.c"  // フォント実体を取り込む
#include "Montserrat.c"
extern const lv_font_t Montserrat;



lv_obj_t* clock_label = nullptr;
extern void create_color_button_screen();

void go_to_button_screen(lv_event_t *e) {
    create_color_button_screen();
}

void create_clock_screen() {
    lv_obj_clean(lv_scr_act());

    clock_label = lv_label_create(lv_scr_act());
    lv_label_set_text(clock_label, "00:00");  // テキストは先に
    lv_obj_set_style_text_font(clock_label, &Montserrat, 0);
    lv_obj_set_style_transform_zoom(clock_label, 384, 0);  // 拡大は後に
    lv_obj_align(clock_label, LV_ALIGN_CENTER, 0, -30);

    lv_obj_t *next_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(next_btn, 100, 50);
    lv_obj_align(next_btn, LV_ALIGN_CENTER, 0, 60);
    lv_obj_t *lbl = lv_label_create(next_btn);
    lv_label_set_text(lbl, "Next");
    lv_obj_center(lbl);
    lv_obj_add_event_cb(next_btn, go_to_button_screen, LV_EVENT_CLICKED, NULL);
}

