#ifndef UI_COMPONENTS_HPP
#define UI_COMPONENTS_HPP

#include <LovyanGFX.hpp>
#include <functional>

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ST7796 _panel;
  lgfx::Bus_SPI _bus;
  lgfx::Light_PWM _light;
  lgfx::Touch_XPT2046 _touch;

public:
  LGFX(void);
};

class MyButton {
public:
  int16_t x, y, w, h;
  uint16_t outlinecolor, fillcolor, textcolor;
  const char* label;
  uint8_t textsize;
  LGFX* gfx;
  std::function<void()> callback;

  void initButton(LGFX* gfx_, int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                  uint16_t outline, uint16_t fill, uint16_t text,
                  const char* label_, uint8_t textsize_);
  void setCallback(std::function<void()> cb);
  void trigger();
  void drawButton(bool inverted = false);
  bool contains(int16_t tx, int16_t ty);
};

#endif
