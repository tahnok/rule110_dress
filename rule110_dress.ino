#include <string.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>

#define PIN 6
#define ROWS 8
#define COLUMNS 42
#define NUM_PINS ROWS * COLUMNS

#define WIDTH ROWS
#define HEIGHT COLUMNS

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PINS, PIN, NEO_GRB + NEO_KHZ800);


extern Adafruit_BluefruitLE_SPI ble;
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
extern uint8_t packetbuffer[];

byte start[] = { 1, 0, 1, 1, 1, 0, 0, 1};
//byte start[WIDTH] = {0};

byte* row1;
byte* row2;
uint32_t colour1;
uint32_t colour2;


void error() {
  pinMode(13, OUTPUT);
  while (1) {
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);
  }
}

void setup() {
  if (!ble.begin(false)) {
    error();
  }

  if (!ble.factoryReset()) {
    error();
  }

  ble.setMode(BLUEFRUIT_MODE_DATA);

  strip.begin();
  strip.setBrightness(16);
  strip.show();

  row1 = start;
  row2 = (byte*) calloc(WIDTH, sizeof(byte));

  colour1 = Wheel(25);
  colour2 = Wheel(100);
}

byte index = 0;

boolean colourFlop;

void loop() {

  if (ble.available()) {
    uint8_t len = readPacket(&ble, 1000);
    if (len == 0) return;
    if (packetbuffer[1] == 'C') {
      uint8_t red = packetbuffer[2];
      uint8_t green = packetbuffer[3];
      uint8_t blue = packetbuffer[4];
      if (colourFlop) {
        colour1 = strip.Color(red, green, blue);
      } else {
        colour2 = strip.Color(red, green, blue);
      }
      colourFlop = !colourFlop;
      resetDisplay();
    }
  } else {
    if (index & 1) {
      displayRow(row2, index);
      apply(row2, row1);
    } else {
      displayRow(row1, index);
      apply(row1, row2);
    }

    if (index >= HEIGHT - 1) {
      resetDisplay();
    } else {
      index = index + 1;
    }
    delay(100);
  }
}

void resetDisplay() {
  for (uint16_t i = 0; i < NUM_PINS; i++) {
    strip.setPixelColor(i, 0);
  }
  index = 0;
}

void displayRow(byte* row, byte column) {
  for (byte i = 0; i < WIDTH; i++) {
    if (row[i]) {
      setColour(i, column, colour1);
    } else {
      setColour(i, column, colour2);
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
