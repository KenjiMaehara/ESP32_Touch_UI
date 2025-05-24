#include "color_button_page.h"
#include <Arduino.h>
#include <lvgl.h>

// 各色の専用コールバック関数
void color_red_cb(lv_event_t *e) {
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0xFF0000), 0);
}
void color_green_cb(lv_event_t *e) {
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x00FF00), 0);
}
void color_blue_cb(lv_event_t *e) {
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x0000FF), 0);
}
void color_yellow_cb(lv_event_t *e) {
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0xFFFF00), 0);
}

void create_color_button_screen() {
    lv_obj_clean(lv_scr_act());

    // ボタンラベルとコールバックの組み合わせ
    struct {
        const char* label;
        lv_event_cb_t cb;
    } buttons[] = {
        {"Red", color_red_cb},
        {"Green", color_green_cb},
        {"Blue", color_blue_cb},
        {"Yellow", color_yellow_cb}
    };

    for (int i = 0; i < 4; i++) {
        lv_obj_t *btn = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn, 100, 50);
        lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 10 + i * 110, 10);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, buttons[i].label);
        lv_obj_center(lbl);
        lv_obj_add_event_cb(btn, buttons[i].cb, LV_EVENT_CLICKED, NULL);  // user_data 不使用
    }

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0xFF0000), 0);  // 初期色：Red
}
