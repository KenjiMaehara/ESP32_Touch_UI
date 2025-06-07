#include "color_button_page.h"
#include "japanese_font_page.h"  // 追加（日本語画面へ遷移用）
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

// 日本語テスト画面に遷移するコールバック
void show_japanese_cb(lv_event_t *e) {
    //create_japanese_font_screen();  // 日本語画面呼び出し
}

void create_color_button_screen() {
    lv_obj_clean(lv_scr_act());

    // 色変更ボタンの定義
    struct {
        const char* label;
        lv_event_cb_t cb;
    } buttons[] = {
        {"Red", color_red_cb},
        {"Green", color_green_cb},
        {"Blue", color_blue_cb},
        {"Yellow", color_yellow_cb}
    };

    // 色変更ボタン配置
    for (int i = 0; i < 4; i++) {
        lv_obj_t *btn = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn, 100, 50);
        lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 10 + i * 110, 10);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, buttons[i].label);
        lv_obj_center(lbl);
        lv_obj_add_event_cb(btn, buttons[i].cb, LV_EVENT_CLICKED, NULL);
    }

    // 日本語テストボタンを追加（画面中央下に配置）
    lv_obj_t *jp_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(jp_btn, 220, 60);
    lv_obj_align(jp_btn, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_t *jp_lbl = lv_label_create(jp_btn);
    lv_label_set_text(jp_lbl, "日本語表示テスト");
    lv_obj_center(jp_lbl);
    lv_obj_add_event_cb(jp_btn, show_japanese_cb, LV_EVENT_CLICKED, NULL);

    // 初期色
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0xFF0000), 0);
}
