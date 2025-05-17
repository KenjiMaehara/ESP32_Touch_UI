# ESP32 + LVGL タッチ UI テスト

## 概要

このプロジェクトは、ESP32-32E モジュールと ST7796 LCD + XPT2046 タッチを使って  
LVGL + LovyanGFX によるタッチUIを構築しようとしています。

## 使用構成

- MCU: ESP32-WROOM-32E
- LCD: 3.5インチ ST7796（SPI）
- タッチ: XPT2046（SPI, CS=IO33）
- ライブラリ: LovyanGFX, LVGL v8

## 現在の問題（ご意見ください！）

- `tft.getTouch()` 単体ではタッチ座標は取得できています
- しかし、`LVGL` の `my_touchpad_read()`（read_cb）が一度も呼ばれません
- `loop()` は動作、`lv_timer_handler()` も呼ばれていることは確認済み
- `flush_cb` は設定済みですが呼ばれているか不明です

### ご協力のお願い

- `LVGL + LovyanGFX` 環境でタッチが動かない原因に心当たりのある方、
  `main.cpp` をレビューいただけると幸いです！

## 使用方法

```bash
git clone https://github.com/yourusername/esp32-lvgl-touch-test.git
pio run -t upload


## Contributing

Pull requests are welcome.  
If you have ideas or fixes, feel free to fork and contribute!

Please make sure any changes are clearly explained and tested.