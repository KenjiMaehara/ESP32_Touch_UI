#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  delay(500);  // 安定化のための短い待機
  Serial.println("setup() start");

  pinMode(27, OUTPUT);       // バックライトON
  digitalWrite(27, HIGH);
  Serial.println("Backlight ON");

  tft.init();
  Serial.println("TFT initialized");

  tft.setRotation(0);
  Serial.println("Rotation set");

  tft.fillScreen(TFT_RED);
  Serial.println("Screen filled RED");

  tft.setCursor(60, 100);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("Hello!");
  Serial.println("Text printed");

  Serial.println("setup() complete");
}


void loop() {
}
