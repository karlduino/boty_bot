#include <Arduino.h>

/***********************************************************************/
/* definitions and variables for Ping                                  */
/***********************************************************************/
const int pingPin = 19;
long lastPingTime = 0;
const int pingDigitPin = 15;
const int pingDigitResetPin = 14;
#define MIN_DISTANCE 8
#define MIN_PING_DELAY 1500
#define MIN_PING_DELAY_WHILE_FORWARD 250

float ping_inch(int ping_pin, int max_readings, boolean display_result);
boolean is_close(float ping_distance, int motorspeed, int motordirection);
void ping_display(float value);
