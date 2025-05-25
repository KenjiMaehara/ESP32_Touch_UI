#include <lvgl.h>
#include "clock_page.h"
#include "MontserratBold64.h"

extern const lv_font_t MontserratBold64;

lv_obj_t* clock_label = nullptr;

void create_clock_screen() {
    lv_obj_clean(lv_scr_act());

    // ラベルラッパー
    lv_obj_t* wrapper = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(wrapper);
    lv_obj_set_size(wrapper, 480, 240);  // 画面いっぱいに確保
    lv_obj_align(wrapper, LV_ALIGN_CENTER, 0, 0);

    // 時計ラベル
    clock_label = lv_label_create(wrapper);
    lv_label_set_text(clock_label, "00:00");
    lv_obj_set_style_text_font(clock_label, &MontserratBold64, 0);
    lv_obj_set_style_text_color(clock_label, lv_color_hex(0xFFFFFF), 0); // 白文字
    lv_obj_center(clock_label);

    // デバッグ用にラベル枠を表示
    lv_obj_set_style_border_width(clock_label, 2, 0);
    lv_obj_set_style_border_color(clock_label, lv_color_hex(0xFF0000), 0);
}
