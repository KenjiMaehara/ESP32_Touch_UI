
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <TouchScreen.h>

TFT_eSPI tft = TFT_eSPI();

// バックライト制御ピン
#define BACKLIGHT_PIN 27

// タッチパネルピン定義（ESP32-32E対応）
#define XP 33
#define XM 26
#define YP 25
#define YM 27

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 500);

static lv_color_t buf[LV_HOR_RES_MAX * 10];
static lv_disp_draw_buf_t draw_buf;

// 画面描画フラッシュ関数
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, false);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

// タッチ読み取り関数
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    TSPoint p = ts.getPoint();

    // デバッグ出力
    Serial.print("z=");
    Serial.print(p.z);
    Serial.print(" x=");
    Serial.print(p.x);
    Serial.print(" y=");
    Serial.println(p.y);

    if (p.z > 300 && p.z < 1000) {
        int x = map(p.y, 200, 3800, 0, 480);  // 必要に応じて調整
        int y = map(p.x, 300, 3700, 0, 320);
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

// ボタンイベント
void btn_event_cb(lv_event_t *e) {
    lv_obj_t *label = (lv_obj_t *)lv_event_get_user_data(e);
    lv_label_set_text(label, "PRESSED!");
}

void setup() {
    Serial.begin(115200);

    pinMode(BACKLIGHT_PIN, OUTPUT);
    digitalWrite(BACKLIGHT_PIN, HIGH);

    tft.begin();
    tft.setRotation(1);  // 横型表示
    tft.fillScreen(TFT_BLACK);

    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, LV_HOR_RES_MAX * 10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 320;
    lv_disp_drv_register(&disp_drv);

    // タッチ入力デバイス登録
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    // ボタン
    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "Click me!");
    lv_obj_center(label);

    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, label);
}

void loop() {
    lv_timer_handler();
    delay(5);
}
