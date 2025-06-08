// screen_signalTest.cpp
#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <functional>
#include "ui_components.hpp"

extern LGFX tft;
extern void drawNextButton(int x, int y, int w, int h);
extern lgfx::touch_point_t tp;

struct RoundButton {
  int x, y, r;
  bool isOn;
  std::function<void()> callback;

  void draw(LGFX* gfx) {
    uint16_t color = isOn ? TFT_RED : TFT_GREEN;
    gfx->fillCircle(x, y, r, color);
  }

  bool contains(int tx, int ty) {
    return sqrt((tx - x) * (tx - x) + (ty - y) * (ty - y)) <= r;
  }

  void toggle(LGFX* gfx) {
    isOn = !isOn;
    draw(gfx);
    if (callback) callback();
  }
};

RoundButton signalButtons[2][4];

void showScreenSignalTest() {
  tft.fillScreen(TFT_PURPLE);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(40, 30);
  tft.print("Screen 3");

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 4; ++j) {
      int cx = 70 + j * 80;
      int cy = 120 + i * 80;
      signalButtons[i][j] = {cx, cy, 30, false};
      signalButtons[i][j].draw(&tft);
    }
  }

  drawNextButton(240, 260, 60, 40);
}

void handleScreenSignalTestTouch() {
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (signalButtons[i][j].contains(tp.x, tp.y)) {
        signalButtons[i][j].toggle(&tft);
        return;
      }
    }
  }
}
