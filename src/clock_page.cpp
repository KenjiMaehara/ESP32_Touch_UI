#include <lvgl.h>
#include "clock_page.h"
//#include "MontserratBold64.h"

//extern const lv_font_t MontserratBold64;
extern const lv_font_t NotoSansBold64;

lv_obj_t* clock_label = nullptr;

void create_clock_screen() {
    lv_obj_clean(lv_scr_act());

    // ラベルのラッパーを作成（全画面サイズに設定）
    lv_obj_t* wrapper = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(wrapper);
    lv_obj_set_size(wrapper, 480, 320);
    lv_obj_align(wrapper, LV_ALIGN_CENTER, 0, 0);

    // 時計ラベルを作成
    clock_label = lv_label_create(wrapper);
    lv_obj_set_style_bg_color(wrapper, lv_color_hex(0x000000), 0); // 真っ黒背景


    // フォントを設定（重要：lv_label_set_text より前に）
    //lv_obj_set_style_text_font(clock_label, &MontserratBold64, 0);
    lv_obj_set_style_text_font(clock_label, &NotoSansBold64, 0);

    // テキストカラー
    lv_obj_set_style_text_color(clock_label, lv_color_hex(0xFFFFFF), 0);

    // テキストを設定
    lv_label_set_text(clock_label, "00:00");

    // 十分なサイズを指定（省略可能だがデバッグしやすい）
    lv_obj_set_size(clock_label, 460, 180);  // 高さをもう少し大きくしてみる

    // 中央に配置
    lv_obj_center(clock_label);

    // デバッグ用：赤い枠線でラベルを囲む
    lv_obj_set_style_border_width(clock_label, 2, 0);
    lv_obj_set_style_border_color(clock_label, lv_color_hex(0xFF0000), 0);
}
