#include <Arduino.h>  // ← この行を追加
#include <lvgl.h>
#include "clock_page.h"
#include "NotoSansBold64.h"  // extern 宣言を含むヘッダー

lv_obj_t* clock_label = nullptr;

void create_clock_screen() {
    lv_obj_clean(lv_scr_act());

    lv_obj_t* wrapper = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(wrapper);
    lv_obj_set_size(wrapper, 480, 320);
    lv_obj_align(wrapper, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(wrapper, lv_color_hex(0x000000), 0);

    clock_label = lv_label_create(wrapper);
    lv_label_set_long_mode(clock_label, LV_LABEL_LONG_CLIP);

    // フォントを仮に小さくする（NotoSansBold64 が大きすぎるかも）
    extern const lv_font_t lv_font_montserrat_32;  // 小さいフォント
    lv_obj_set_style_text_font(clock_label, &lv_font_montserrat_32, 0);  // ← ここを差し替え
    lv_obj_set_style_text_color(clock_label, lv_color_hex(0xFFFFFF), 0);

    lv_label_set_text(clock_label, "00:00");

    lv_obj_center(clock_label);

    lv_obj_set_style_border_width(clock_label, 2, 0);
    lv_obj_set_style_border_color(clock_label, lv_color_hex(0xFF0000), 0);
}

