#include <Adafruit_NeoPixel.h>

#define PIN 6
#define ROWS 8
#define COLUMNS 42
#define NUM_PINS ROWS * COLUMNS

#define WIDTH ROWS
#define HEIGHT COLUMNS

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PINS, PIN, NEO_GRB + NEO_KHZ800);

byte start[] = { 0, 1, 0, 0, 1, 0, 1, 0};
//byte start[WIDTH] = {0};

byte* row1;
byte* row2;

void setup() {
  strip.begin();
  strip.setBrightness(16);
  strip.show();

  row1 = start;
  row2 = (byte*) malloc(sizeof(byte) * WIDTH);
}

byte index = 0;

void loop() {
  if (index & 1) {
    displayRow(row2, index);
    apply(row2, row1);
  } else {
    displayRow(row1, index);
    apply(row1, row2);

  }
  if (index >= HEIGHT - 1) {
    index = 0;
    for(;;);
  } else {
    index = index + 1;
  }
  delay(100);
}

void displayRow(byte* row, byte column) {
  for (byte i = 0; i < WIDTH; i++) {
    if (row[i]) {
      setColour(i, column, Wheel(25));
    } else {
      setColour(i, column, Wheel(100));
    }
  }
  strip.show();
}

void apply(byte top[WIDTH], byte bottom[WIDTH]) {
  for (byte i = 0; i < WIDTH; i++) {
    if (i == 0 || i == WIDTH - 1) {
      bottom[i] = top[i];
    } else {
      if ( (top[i - 1] && top[i] && !top[i + 1]) ||
           (top[i - 1] && !top[i] && top[i + 1]) ||
           (!top[i - 1] && top[i] && top[i + 1]) ||
           (!top[i - 1] && top[i] && !top[i + 1]) ||
           (!top[i - 1] && !top[i] && top[i + 1])
         ) {
        bottom[i] = 1;
      } else {
        bottom[i] = 0;
      }
    }

  }
}

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
