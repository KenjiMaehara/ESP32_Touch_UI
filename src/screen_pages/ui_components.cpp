#include "ui_components.hpp"

void MyButton::initButton(LGFX* gfx_, int16_t x_, int16_t y_, int16_t w_, int16_t h_,
                          uint16_t outline, uint16_t fill, uint16_t text,
                          const char* label_, uint8_t textsize_) {
  gfx = gfx_;
  x = x_ - w_ / 2;
  y = y_ - h_ / 2;
  w = w_;
  h = h_;
  outlinecolor = outline;
  fillcolor = fill;
  textcolor = text;
  label = label_;
  textsize = textsize_;
}

void MyButton::setCallback(std::function<void()> cb) {
  callback = cb;
}

void MyButton::trigger() {
  if (callback) callback();
}

void MyButton::drawButton(bool inverted) {
  uint16_t fill = inverted ? textcolor : fillcolor;
  uint16_t text = inverted ? fillcolor : textcolor;
  gfx->fillRect(x, y, w, h, fill);
  gfx->drawRect(x, y, w, h, outlinecolor);
  int16_t x_c = x + (w / 2) - (strlen(label) * 6 * textsize / 2);
  int16_t y_c = y + (h / 2) - (8 * textsize / 2);
  gfx->setTextColor(text);
  gfx->setTextSize(textsize);
  gfx->setCursor(x_c, y_c);
  gfx->print(label);
}

bool MyButton::contains(int16_t tx, int16_t ty) {
  return (tx >= x && tx < (x + w) && ty >= y && ty < (y + h));
}
