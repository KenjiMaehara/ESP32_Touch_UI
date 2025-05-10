#include <TFT_eSPI.h>
#include <SPI.h>
#include <TouchScreen.h>

TFT_eSPI tft = TFT_eSPI();

// タッチピン設定（ESP32-32Eの配線に合わせる）
#define XP 33
#define XM 32
#define YP 34
#define YM 35

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);  // 最後の数字は圧力しきい値

void setup() {
  Serial.begin(115200);
  pinMode(27, OUTPUT);  // バックライトON
  digitalWrite(27, HIGH);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(40, 40);
  tft.println("Touch the screen!");
}

void loop() {
  TSPoint p = ts.getPoint();

  if (p.z > 50) {  // ある程度の圧がかかったときだけ反応
    Serial.print("X = "); Serial.print(p.x);
    Serial.print(" | Y = "); Serial.print(p.y);
    Serial.print(" | Pressure = "); Serial.println(p.z);

    tft.fillCircle(p.x / 10, p.y / 10, 3, TFT_RED);  // 簡易的に表示位置調整
    delay(100);
  }
}
