#include <lvgl.h>
#include "clock_page.h"
#include "MontserratBold64.h"  // フォントデータを直接インクルード

extern const lv_font_t lv_font_montserrat_64;

lv_obj_t* clock_label = nullptr;
extern void create_color_button_screen();

void delayed_screen_switch(lv_timer_t *timer) {
    create_color_button_screen();
    lv_timer_del(timer);
}

void go_to_button_screen(lv_event_t *e) {
    lv_timer_create(delayed_screen_switch, 10, NULL);
}

void create_clock_screen() {
    lv_obj_clean(lv_scr_act());

    // ラベルラッパー
    lv_obj_t *wrapper = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(wrapper);
    lv_obj_set_size(wrapper, 300, 120);
    lv_obj_align(wrapper, LV_ALIGN_CENTER, 0, -50);

    // ラベル本体
    clock_label = lv_label_create(wrapper);
    lv_label_set_text(clock_label, "00:00");
    lv_obj_set_style_text_font(clock_label, &MontserratBold64, 0);  // 修正済み
    lv_obj_center(clock_label);

    // Nextボタン
    lv_obj_t *next_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(next_btn, 100, 50);
    lv_obj_align(next_btn, LV_ALIGN_CENTER, 0, 60);
    lv_obj_t *lbl = lv_label_create(next_btn);
    lv_label_set_text(lbl, "Next");
    lv_obj_center(lbl);
    lv_obj_add_event_cb(next_btn, go_to_button_screen, LV_EVENT_CLICKED, NULL);
}
