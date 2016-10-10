#include "shared.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PINS, PIN, NEO_GRB + NEO_KHZ800);


void setColour(byte x, byte y, uint32_t colour) {
  uint16_t pos;
  if (x & 1) {
    pos = (ROWS - x - 1) * COLUMNS + y;

  } else {
    pos = (ROWS - x - 1) * COLUMNS + (COLUMNS - y - 1);

  }
  strip.setPixelColor(pos, colour);
}

uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
