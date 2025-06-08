// tenkey.cpp
#include <Arduino.h>
#include "ui_components.hpp"
#include "tenkey.hpp"
#include "screen_signalTest.hpp"

extern LGFX tft;
extern String input_value;
extern void drawNextButton(int x, int y, int w, int h);
extern const char* keys[4][3];
extern MyButton keyButtons[4][3];
extern int screen_state;

void showScreen3() {
  tft.fillScreen(TFT_DARKGREY);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(40, 20);
  tft.print("Input: " + input_value);

  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 3; col++) {
      int x = 40 + col * 80;
      int y = 40 + row * 70;
      keyButtons[row][col].initButton(&tft, x + 30, y + 30, 60, 60, TFT_BLACK, TFT_WHITE, TFT_BLACK, keys[row][col], 2);
      keyButtons[row][col].setCallback([row, col]() {
        input_value += keys[row][col];
        showScreen3();
      });
      keyButtons[row][col].drawButton();
    }
  }

  MyButton* next = new MyButton();
  next->initButton(&tft, 270, 280, 80, 40, TFT_BLACK, TFT_GREEN, TFT_BLACK, "Next", 2);
  next->setCallback([]() {
    screen_state = 4;
    showScreenSignalTest();
  });
  next->drawButton();
}
