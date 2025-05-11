
#include <lvgl.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define BACKLIGHT_PIN 27

// バッファサイズ
static lv_color_t buf[LV_HOR_RES_MAX * 10];
static lv_disp_draw_buf_t draw_buf;

// 描画フラッシュ関数
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, false);  // 同期描画
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

void setup() {
    Serial.begin(115200);

    // バックライトON
    pinMode(BACKLIGHT_PIN, OUTPUT);
    digitalWrite(BACKLIGHT_PIN, HIGH);

    // TFT 初期化
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    // LVGL 初期化
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, LV_HOR_RES_MAX * 10);

    // LVGLディスプレイドライバ登録
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res = 320;
    disp_drv.ver_res = 480;
    lv_disp_drv_register(&disp_drv);

    // ボタン作成
    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "Click me!");
    lv_obj_center(label);
}

void loop() {
    lv_timer_handler();  // LVGL処理ループ
    delay(5);
}
