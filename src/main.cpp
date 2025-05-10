#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

#define BUTTON_X  60
#define BUTTON_Y  100
#define BUTTON_W  200
#define BUTTON_H  60

bool buttonPressed = false;

void drawButton(bool pressed) {
  uint16_t color = pressed ? TFT_GREEN : TFT_BLUE;
  tft.fillRect(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, color);
  tft.drawRect(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, TFT_WHITE);
  tft.setTextColor(TFT_WHITE, color);
  tft.setTextDatum(MC_DATUM);  // 中央揃え
  tft.drawString("Press Me", BUTTON_X + BUTTON_W / 2, BUTTON_Y + BUTTON_H / 2);
}

void setup() {
  Serial.begin(115200);
  pinMode(27, OUTPUT);  // バックライトON（必要に応じて変更）
  digitalWrite(27, HIGH);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  drawButton(false);  // 初期ボタン表示
}

void loop() {
  // デモのため、自動で色を変える
  delay(2000);
  buttonPressed = !buttonPressed;
  drawButton(buttonPressed);
}
