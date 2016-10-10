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


extern Adafruit_BluefruitLE_SPI ble;
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
extern uint8_t packetbuffer[];


extern Adafruit_NeoPixel strip;
void setColour(byte x, byte y, uint32_t colour);
uint32_t Wheel(byte WheelPos);


void displayRow(byte* row, byte column);
void apply(byte top[WIDTH], byte bottom[WIDTH]);
