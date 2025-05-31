#include <Arduino.h>
#include <lvgl.h>
#include "clock_page.h"
#include "NotoSansBold64.h"

lv_obj_t* clock_label = nullptr;
extern void create_color_button_screen();

// 遅延実行用コールバック（未使用）
void delayed_screen_switch(lv_timer_t *timer) {
    create_color_button_screen();
    lv_timer_del(timer);
}

// ボタンのコールバック → カラー選択ページに直接遷移
void go_to_button_screen(lv_event_t *e) {
    create_color_button_screen();
}

void create_clock_screen() {
    lv_obj_clean(lv_scr_act());

    lv_obj_t* wrapper = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(wrapper);
    lv_obj_set_size(wrapper, 480, 320);
    lv_obj_align(wrapper, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(wrapper, lv_color_hex(0xFFFFFF), 0);  // 白背景

    clock_label = lv_label_create(wrapper);
    lv_label_set_long_mode(clock_label, LV_LABEL_LONG_CLIP);
    lv_label_set_text(clock_label, "12:34");

    // フォントと色を設定
    lv_obj_set_style_text_font(clock_label, &NotoSansBold64, 0);
    lv_obj_set_style_text_color(clock_label, lv_color_hex(0x000000), 0);

    // 中央に配置
    lv_obj_align(clock_label, LV_ALIGN_CENTER, 0, 0);

    // デバッグ用ボーダー
    lv_obj_set_style_border_width(clock_label, 2, 0);
    lv_obj_set_style_border_color(clock_label, lv_color_hex(0xFF0000), 0);

    // Nextボタンの作成と配置
    lv_obj_t* next_btn = lv_btn_create(wrapper);
    lv_obj_set_size(next_btn, 100, 50);
    lv_obj_align(next_btn, LV_ALIGN_BOTTOM_MID, 0, -10);

    lv_obj_t* next_label = lv_label_create(next_btn);
    lv_label_set_text(next_label, "Next");
    lv_obj_center(next_label);

    lv_obj_add_event_cb(next_btn, go_to_button_screen, LV_EVENT_CLICKED, NULL);
}
