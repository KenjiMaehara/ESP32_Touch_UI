#include <Arduino.h>     // ← これを追加！
#include <lvgl.h>
#include "lvgl_main.h"

void setup() {
  lvgl_setup();  // UI初期化
}

void loop() {
  lv_timer_handler();  // LVGL内部イベント処理
  delay(5);
}
