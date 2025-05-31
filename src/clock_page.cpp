#include <Arduino.h>
#include <lvgl.h>
#include "clock_page.h"
#include "NotoSansBold64.h"

lv_obj_t* clock_label = nullptr;

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

    // フォントと色を設定（サイズ指定なし）
    lv_obj_set_style_text_font(clock_label, &NotoSansBold64, 0);
    lv_obj_set_style_text_color(clock_label, lv_color_hex(0x000000), 0);

    // サイズ指定を削除 → 自動サイズ調整に任せる
    // lv_obj_set_width(clock_label, 480);
    // lv_obj_set_height(clock_label, 100);

    // 正確な中央配置にする
    lv_obj_align(clock_label, LV_ALIGN_CENTER, 0, 0);

    // デバッグ用ボーダー
    lv_obj_set_style_border_width(clock_label, 2, 0);
    lv_obj_set_style_border_color(clock_label, lv_color_hex(0xFF0000), 0);
}
