#include <Arduino.h>
#include <Servo.h>

/***********************************************************************/
/* servo functions                                                     */
/***********************************************************************/
void move_servo_slowly(Servo servo, int new_angle, int n_steps, int delay_betw_steps)
{
  int i;
  int cur_angle = servo.read();
  new_angle += SERVO_CORRECTION;
  
  for(i=1; i<=n_steps; i++) {
    servo.write((new_angle - cur_angle)*i/n_steps + cur_angle);
    delay(delay_betw_steps);
  }
}

void random_servo_angle(Servo servo)
{
  int cur_pos, new_pos;
  if(millis() < lastRandomServoMove + random(5000, 10000)) {
    if(motordirection == FORWARD) servo.write(90+SERVO_CORRECTION);
    return;
  }
  lastRandomServoMove = millis();

  cur_pos = servo.read();
  new_pos = random(cur_pos - 45, cur_pos + 45);
  new_pos = constrain(new_pos, SERVO_ANGLE_MIN+SERVO_CORRECTION, 
		      SERVO_ANGLE_MAX+SERVO_CORRECTION);

  servo.write(new_pos);
  if(motordirection == FORWARD) {
    delay(500);
    servo.write(90+SERVO_CORRECTION);
  }

#ifdef DEBUG
  Serial.print("Servo angle: ");
  Serial.println(servo.read());
#endif
}

int scan_and_ping(Servo servo, int n_steps, int n_smooth)
{
  int i, j, delay_amount;
  int angles[n_steps*2+1], *left_angles, max_angle;
  float readings[n_steps*2+1], *left_readings, max_reading, cur_reading;
  boolean left_first = false;
  

  delay_amount = (SERVO_ANGLE_MAX - SERVO_ANGLE_MIN)*7.5/(n_steps*2+1);
#ifdef DEBUG
  Serial.println("Scan and ping");
  Serial.print("Delay amount = ");
  Serial.println(delay_amount);
#endif

  left_angles = angles + n_steps;
  left_readings = readings + n_steps;

  for(i=0; i<n_steps; i++) {
    angles[i] = SERVO_CORRECTION + SERVO_ANGLE_MIN + (90 - SERVO_ANGLE_MIN)*i/n_steps;
    left_angles[i] = SERVO_CORRECTION + 90 + (SERVO_ANGLE_MAX - 90)*i/n_steps;
  }
  left_angles[n_steps] = SERVO_ANGLE_MAX;

  if(random(1,3) == 1) { // look left first
    left_first = true;
    for(i=0; i<=n_steps; i++) {
      ping_servo.write(left_angles[i]);
      left_readings[i] = ping_inch(pingPin, 1, false);
      delay(delay_amount);
#ifdef DEBUG    
      if(i==0) Serial.println("sweep left");
      Serial.print(left_angles[i]);
      Serial.print("\t");
      Serial.println(left_readings[i]);
#endif
    }
    for(i=0; i<=n_steps; i++) {
      ping_servo.write(left_angles[n_steps-i]);
      left_readings[n_steps-i] += ping_inch(pingPin, 1, false);
      delay(delay_amount);
#ifdef DEBUG    
      if(i==0) Serial.println("back to center");
      Serial.print(left_angles[n_steps-i]);
      Serial.print("\t");
      Serial.println(left_readings[n_steps-i]);
#endif
    }
  }

  for(i=0; i<n_steps; i++) {
    ping_servo.write(angles[n_steps-1-i]);
    readings[n_steps-1-i] = ping_inch(pingPin, 1, false);
    delay(delay_amount);
#ifdef DEBUG    
    if(i==0) Serial.println("sweep right");
    Serial.print(angles[n_steps-1-i]);
    Serial.print("\t");
    Serial.println(readings[n_steps-1-i]);
#endif
  }
  for(i=0; i<n_steps; i++) {
    ping_servo.write(angles[i]);
    readings[i] += ping_inch(pingPin, 1, false);
    delay(delay_amount);
#ifdef DEBUG    
    if(i==0) Serial.println("back to center");
    Serial.print(angles[i]);
    Serial.print("\t");
    Serial.println(readings[i]);
#endif
  }

  if(!left_first) {
    for(i=0; i<=n_steps; i++) {
      ping_servo.write(left_angles[i]);
      left_readings[i] = ping_inch(pingPin, 1, false);
      delay(delay_amount);
#ifdef DEBUG    
      if(i==0) Serial.println("sweep left");
      Serial.print(left_angles[i]);
      Serial.print("\t");
      Serial.println(left_readings[i]);
#endif
    }
    for(i=0; i<=n_steps; i++) {
      ping_servo.write(left_angles[n_steps-i]);
      left_readings[n_steps-i] += ping_inch(pingPin, 1, false);
      delay(delay_amount);
#ifdef DEBUG    
      if(i==0) Serial.println("back to center");
      Serial.print(left_angles[n_steps-i]);
      Serial.print("\t");
      Serial.println(left_readings[n_steps-i]);
#endif
    }
  }
  
  ping_servo.write(90+SERVO_CORRECTION);

  // smooth and minimize
  max_reading = -999.0;
  Serial.println("Results:");
  for(i=0; i<n_steps*2+1; i++) {
    cur_reading = 0.0;
    for(j = i-n_smooth; j<i+n_smooth; j++) {
      if(j < 0) cur_reading += readings[0];
      else if(j > n_steps*2-1) cur_reading += readings[n_steps*2-1];
      else cur_reading += readings[j];
    }
    cur_reading /= (float)(n_smooth*2+1);
#ifdef DEBUG
    Serial.print(angles[i]);
    Serial.print("\t");
    Serial.print(readings[i]);
    Serial.print("\t");
    Serial.println(cur_reading);
#endif
    if(cur_reading > max_reading) {
      max_reading = cur_reading;
      max_angle = angles[i];
    }
  }
#ifdef DEBUG
  Serial.print("best angle: ");
  Serial.print(max_angle);
  Serial.print("\t");
  Serial.println(max_reading);
#endif

  return(max_angle);
}