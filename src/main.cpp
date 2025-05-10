#include <TFT_eSPI.h>
#include <SPI.h>
#include <TouchScreen.h>

TFT_eSPI tft = TFT_eSPI();

// タッチピン設定（ESP32-32Eの配線に合わせる）
//#define XP 33
//#define XM 32
//#define YP 34
//#define YM 35

#define XP 27
#define XM 26
#define YP 25
#define YM 33

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 500);  // 最後の数字は圧力しきい値

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

void resetTouchPins() {
  // XM = GPIO32 → 出力OK
  // XP = GPIO33 → 出力OK
  // YP = GPIO34 → 入力専用
  // YM = GPIO35 → 入力専用

  pinMode(XM, OUTPUT);  // OK
  pinMode(XP, OUTPUT);  // OK
  pinMode(YP, INPUT);   // 必ずINPUT
  pinMode(YM, INPUT);   // 必ずINPUT
}

void loop() {
  resetTouchPins();  // ← 重要！

  TSPoint p = ts.getPoint();
  Serial.println(p.z);  // ← zが常に0なら未接続

  //if (p.z > 400 && p.z < 900 && p.x > 50 && p.x < 950 && p.y > 50 && p.y < 950) {
  if (p.z > 400 && p.z < 900) {
    Serial.println("Touch detected");
    Serial.print("X = "); Serial.print(p.x);
    Serial.print(" | Y = "); Serial.print(p.y);
    Serial.print(" | Pressure = "); Serial.println(p.z);

    tft.fillCircle(p.x / 10, p.y / 10, 3, TFT_RED);
    delay(100);
  }

}
