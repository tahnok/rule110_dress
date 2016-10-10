#include "shared.h"

byte start[] = { 1, 0, 1, 1, 1, 0, 0, 1};
//byte start[WIDTH] = {0};

byte* row1;
byte* row2;
uint32_t colour1;
uint32_t colour2;
byte index = 0;
boolean colourFlop;

void setup() {
  if (!(ble.begin(false) && ble.factoryReset())) {
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

void error() {
  pinMode(13, OUTPUT);
  while (1) {
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);
  }
}

