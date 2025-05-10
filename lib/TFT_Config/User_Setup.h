#define ST7796_DRIVER

#define TFT_WIDTH  320
#define TFT_HEIGHT 480

#define TFT_MISO -1
#define TFT_MOSI 13    // IO13 = MOSI
#define TFT_SCLK 14    // IO14 = SCK
#define TFT_CS   15    // IO15 = CS
#define TFT_DC   2     // IO2  = DC
#define TFT_RST  -1    // ENピン接続で制御不要

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_GFXFF

#define SPI_FREQUENCY  27000000
