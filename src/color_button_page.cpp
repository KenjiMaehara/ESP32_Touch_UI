#include "color_button_page.h"

void color_btn_event_cb(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);
    lv_color_t *color = (lv_color_t *)lv_event_get_user_data(e);
    lv_obj_set_style_bg_color(lv_scr_act(), *color, 0);
    lv_obj_t *label = lv_obj_get_child(btn, 0);
    const char* txt = lv_label_get_text(label);
    Serial.printf("Button pressed: %s\n", txt);
}

void create_color_button_screen() {
    lv_obj_clean(lv_scr_act());

    static lv_color_t red = lv_color_hex(0xFF0000);
    static lv_color_t green = lv_color_hex(0x00FF00);
    static lv_color_t blue = lv_color_hex(0x0000FF);
    static lv_color_t yellow = lv_color_hex(0xFFFF00);

    const char *btn_labels[] = {"Red", "Green", "Blue", "Yellow"};
    lv_color_t *colors[] = {&red, &green, &blue, &yellow};

    for (int i = 0; i < 4; i++) {
        lv_obj_t *btn = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn, 100, 50);
        lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 10 + i * 110, 10);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, btn_labels[i]);
        lv_obj_center(lbl);
        lv_obj_add_event_cb(btn, color_btn_event_cb, LV_EVENT_CLICKED, colors[i]);
    }

    lv_obj_set_style_bg_color(lv_scr_act(), red, 0);
}
