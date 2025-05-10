//#define ILI9488_DRIVER
#define ST7796_DRIVER

#define TFT_WIDTH  320
#define TFT_HEIGHT 480

#define TFT_MISO -1      // 読み出しなし
#define TFT_MOSI 23
#define TFT_SCLK 18
//#define TFT_CS   15
#define TFT_CS   14
#define TFT_DC   2
#define TFT_RST  -1
//#define TFT_RST  4

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_GFXFF

#define SPI_FREQUENCY  27000000
//#define SPI_FREQUENCY  20000000
//#define SPI_FREQUENCY  16000000
