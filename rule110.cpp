#include "shared.h"

extern uint32_t colour1;
extern uint32_t colour2;

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
