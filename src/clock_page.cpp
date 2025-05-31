#include <lvgl.h>
#include "clock_page.h"
//#include "lv_font_montserrat_48.c"  // フォント実体を取り込む
#include "Montserrat.c"
extern const lv_font_t Montserrat;

lv_obj_t* clock_label = nullptr;
extern void create_color_button_screen();

// 遅延実行用コールバック
void delayed_screen_switch(lv_timer_t *timer) {
    create_color_button_screen();
    lv_timer_del(timer);
}

// ボタンのコールバック → タイマーで非同期遷移
void go_to_button_screen(lv_event_t *e) {
    //lv_timer_create(delayed_screen_switch, 10, NULL);
    create_color_button_screen();  // ← タイマーなしで直接呼び出し
}

void create_clock_screen() {
    lv_obj_clean(lv_scr_act());

    // ラベル用のラッパーオブジェクト
    lv_obj_t *wrapper = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(wrapper);  // 背景や枠を削除
    lv_obj_set_size(wrapper, 240, 100); // ラベルを収めるサイズ
    lv_obj_align(wrapper, LV_ALIGN_CENTER, 0, -30);
    //lv_obj_set_style_transform_zoom(wrapper, 384, 0);  // 1.5倍に拡大

    // ラベル
    clock_label = lv_label_create(wrapper);
    lv_label_set_text(clock_label, "00:00");
    lv_obj_set_style_text_font(clock_label, &Montserrat, 0);
    lv_obj_center(clock_label);

    // ボタン
    lv_obj_t *next_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(next_btn, 100, 50);
    lv_obj_align(next_btn, LV_ALIGN_CENTER, 0, 60);
    lv_obj_t *lbl = lv_label_create(next_btn);
    lv_label_set_text(lbl, "Next");
    lv_obj_center(lbl);
    lv_obj_add_event_cb(next_btn, go_to_button_screen, LV_EVENT_CLICKED, NULL);
}
