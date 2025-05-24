#include "color_button_page.h"
#include <Arduino.h>

void color_btn_event_cb(lv_event_t *e) {
    intptr_t index = (intptr_t)lv_event_get_user_data(e);

    lv_color_t color;
    switch (index) {
        case 0: color = lv_color_hex(0xFF0000); break;
        case 1: color = lv_color_hex(0x00FF00); break;
        case 2: color = lv_color_hex(0x0000FF); break;
        case 3: color = lv_color_hex(0xFFFF00); break;
        default: return;
    }

    lv_obj_set_style_bg_color(lv_scr_act(), color, 0);
}

void create_color_button_screen() {
    lv_obj_clean(lv_scr_act());

    const char *btn_labels[] = {"Red", "Green", "Blue", "Yellow"};

    for (int i = 0; i < 4; i++) {
        lv_obj_t *btn = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn, 100, 50);
        lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 10 + i * 110, 10);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, btn_labels[i]);
        lv_obj_center(lbl);
        lv_obj_add_event_cb(btn, color_btn_event_cb, LV_EVENT_CLICKED, (void*)(intptr_t)i);  // indexを渡す
    }

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0xFF0000), 0);  // 初期色：Red
}

