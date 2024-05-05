#ifndef __MAIN_H_INCLUDE
#define __MAIN_H_INCLUDE

#include <Arduino.h>
#include <Wire.h>
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include "arduino_secrets.h"
#include "WiFiS3.h"
#include "SHT31.h"

#define WIRE_CLOCK 100000

const byte FRAME_ON[8][12] = {
  { 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0 },
  { 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

const byte FRAME_OFF[8][12] = {0};

#define NUM_RESPONSE_LINES 5

const char* const RESPONSE_TEMPLATE = "HTTP/1.1 200 OK\n"
    "Content-type:text/html\n"
    "\n"
    "<p> Temperature: $T&deg;C </p>"
    "<p> Humidity: $H% </p>"
    "<br/>"
    "<img src=\"https://i.giphy.com/THlB4bsoSA0Cc.webp\"></img>"
;

void print_server_status();
void read_temperature_humidity(float& temperature, float& humidity);
void respond_to_client(WiFiClient& client);
void matrix_show_ip_address();
void matrix_print(const char line[]);

#endif