#include <lvgl.h>
#include "NotoSansJPRegular24.h"  // あなたの生成フォントを include

void create_japanese_font_screen() {
    // スクリーン作成
    lv_obj_t *scr = lv_obj_create(NULL);

    // ラベル作成
    lv_obj_t *label = lv_label_create(scr);

    // テキスト設定（日本語）
    lv_label_set_text(label, "こんにちは 世界\n時刻 設定");

    // フォント設定
    lv_obj_set_style_text_font(label, &NotoSansJPRegular24, LV_PART_MAIN);

    // テキスト中央揃え
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    // 画面中央に配置
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    // スクリーンを表示
    lv_scr_load(scr);
}
