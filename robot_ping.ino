#include <Arduino.h>

/***********************************************************************/
/* Ping functions                                                      */
/***********************************************************************/
float ping_inch(int ping_pin, int max_readings, boolean display_result)
{
  int duration;
#define MS_PER_INCH 147.492 // 2 x distance sound travels in a microsecond

  lastPingTime = millis();
  float total=0;
  boolean done=false;
  int n_pings=0;
  
  while(!done) {
    n_pings++;

    // trigger ping
    pinMode(ping_pin, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);

    // read ping
    pinMode(ping_pin, INPUT);
    duration = pulseIn(ping_pin, HIGH);
    
    total +=  (float) duration / MS_PER_INCH;
    if(total > 0 || n_pings >= max_readings) done=true;
  }

  if(display_result) ping_display(total / (float)n_pings);

  return(total / (float)n_pings);
}


boolean is_close(float ping_distance, int motorspeed, int motordirection)
{
  ping_distance = ping_inch(pingPin, 10, true);

  if(ping_distance <= MIN_DISTANCE ||
     (motordirection == FORWARD &&
      ping_distance <= MIN_DISTANCE*(1.0 + ((float)motorspeed - MIN_SPEED)/
				     (MAX_SPEED-MIN_SPEED))))
    return(true);

  return(false);
}



void ping_display_setup(void)
{
  pinMode(pingDigitResetPin, OUTPUT);
  pinMode(pingDigitPin, OUTPUT);
  digitalWrite(pingDigitResetPin, LOW);
  digitalWrite(pingDigitPin, LOW);
}

void ping_display(float inches)
{
  int displayValue = inches/4;
  if(displayValue > 9) displayValue = 9;

  // reset pin
  digitalWrite(pingDigitResetPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingDigitResetPin, LOW);

#ifdef DEBUG
  Serial.print("Ping: ");
  Serial.print(inches);
  Serial.print(" inches\t(");
  Serial.print(inches/12.0);
  Serial.print(" feet)\tDisplay value = ");
  Serial.println(displayValue);
#endif


  for(int i=0; i<displayValue; i++) {
    digitalWrite(pingDigitPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingDigitPin, LOW);
  }
}
