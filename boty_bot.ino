// Arduino program for my first robot
// Karlduino
// first written 2 March 2012
// last modified 26 March 2012

#define DEBUG

#include <AFMotor.h>

#include <Servo.h>
#include "robot_servo.h"

#include "robot_remote.h"
#include "robot_ping.h"
#include "robot_motors.h"

float ping_distance;


void setup(void) {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  pinMode(REMOTEPIN, INPUT);      // Pin for IR sensor
  digitalWrite(REMOTEPIN, HIGH);  // Enable pull-up resistor

  pinMode(led13Pin, OUTPUT);
  digitalWrite(led13Pin, led13State);

  ping_servo.attach(10);  // attached to first servo connector (10)
  ping_servo.write(90+SERVO_CORRECTION);

  set_all_speeds(0, RELEASE);
  full_stop();

  ping_display_setup();
  ping_distance = ping_inch(pingPin, 4, true);
}

void loop(void)
{
  boolean add_delay, change_direction, change_speed;
  int new_angle;
  long curTime;
  
  interpretedIRkey = 0;

  // read IR remote
  if(digitalRead(REMOTEPIN)==LOW) {
    if(IRkey != 0) lastIRkey = IRkey;
    IRkey = getIRkey();
    interpretedIRkey = handleIRkey(IRkey, lastIRkey);

#ifdef DEBUG      
    print_interpretedIRkey(interpretedIRkey);
#endif

    if(interpretedIRkey != 0) {
      led13State = 1 - led13State;
      digitalWrite(led13Pin, led13State);
    }
  }


  // if going forward, ping and potentially stop
  // if not going forward: ping less frequently
  curTime = millis();
  if((motordirection == FORWARD && curTime > lastPingTime + 250) ||
     curTime > lastPingTime + 1000)
    ping_distance = ping_inch(pingPin, 4, true);

  // don't get too close
  if(motordirection == FORWARD && is_close(ping_distance, motorspeed, motordirection)) {
#ifdef DEBUG
    Serial.println("Too close!  Stop.");
#endif
    full_stop();

    new_angle = scan_and_ping(ping_servo, 20, 3);
    if(new_angle < 90+SERVO_CORRECTION) {
#ifdef DEBUG
      Serial.println("Turn right");
#endif
      spin(RIGHT, TURN_TIME*(90+SERVO_CORRECTION-new_angle)/20, TURN_SPEED);
    }
    else {
#ifdef DEBUG
      Serial.println("Turn left");
#endif
      spin(LEFT, TURN_TIME*(new_angle-90-SERVO_CORRECTION)/20, TURN_SPEED);
    }

    // starting moving forward again
    set_all_speeds(motorspeed, motordirection);
    run_all_motors(motordirection);
  }

  add_delay = change_direction = change_speed = false;

  switch(interpretedIRkey) {
  case 0: break;

  case MENU:
      full_stop();
      new_angle = scan_and_ping(ping_servo, 20, 3);
      if(new_angle < 90+SERVO_CORRECTION) {
#ifdef DEBUG
	Serial.println("Turn right");
#endif
	spin(RIGHT, TURN_TIME*(90+SERVO_CORRECTION-new_angle)/20, TURN_SPEED);
      }
      else {
#ifdef DEBUG
	Serial.println("Turn left");
#endif
	spin(LEFT, TURN_TIME*(new_angle-90-SERVO_CORRECTION)/20, TURN_SPEED);
      }

      // starting moving forward again
      if(motorspeed>0 && motordirection != RELEASE) {
	set_all_speeds(motorspeed, motordirection);
	run_all_motors(motordirection);
      }
      else {
	motorspeed = 0; 
	motordirection = RELEASE;
      }
      break;

  case PLAY: // actually stop
    if(motordirection != RELEASE) {
      motordirection = RELEASE;
      change_direction = true;
      add_delay = true;
    }
    if(motorspeed != 0) {
      motorspeed = 0;
      change_speed = true;
    }
    break;

  case UP:
    if(motorspeed<MIN_SPEED || motordirection == RELEASE) {
      motorspeed = MIN_SPEED;
      motordirection = FORWARD;
      change_direction = change_speed = true;
    }
    else {
      if(motordirection == FORWARD) {
	motorspeed += SPEED_INCREMENT;
	change_speed = true;
      }
      else {
	motorspeed -= SPEED_INCREMENT;
	change_speed = true;
	if(motorspeed < MIN_SPEED) {
	  change_direction = true;
	  motordirection = RELEASE;
	  motorspeed = 0;
	  add_delay = true;
	}
      }
    }
    break;

  case DOWN:
    if(motorspeed<MIN_SPEED || motordirection == RELEASE) {
      motorspeed = MIN_SPEED;
      motordirection = BACKWARD;
      change_direction = change_speed = true;
    }
    else {
      if(motordirection == BACKWARD) {
	motorspeed += SPEED_INCREMENT;
	change_speed = true;
      }
      else {
	motorspeed -= SPEED_INCREMENT;
	change_speed = true;
	if(motorspeed < MIN_SPEED) {
	  change_direction = true;
	  motordirection = RELEASE;
	  motorspeed = 0;
	  add_delay = true;
	}
      }
    }
    break;

  case RIGHT:
    motordirection = RELEASE;
    motorspeed = 0;
    change_speed = change_direction = true;
    spin(RIGHT, TURN_TIME, TURN_SPEED);
    break;

  case LEFT:
    motordirection = RELEASE;
    motorspeed = 0;
    change_speed = change_direction = true;
    spin(LEFT, TURN_TIME, TURN_SPEED);
    break;
  }

  if(motorspeed > MAX_SPEED) motorspeed = MAX_SPEED;
  if(change_speed) set_all_speeds(motorspeed, motordirection);
  if(change_direction) run_all_motors(motordirection);

  if(add_delay) { // add a second delay before starting up again; also stop the repeat
    delay(DELAY_AFTER_STOP);
    previousIRkey = 0;

    ping_servo.write(90+SERVO_CORRECTION);
  }

#ifdef DEBUG
  if(millis() - last_motor_speed_report > 1000) {
    last_motor_speed_report = millis();
    Serial.print("direction = ");
    Serial.print(motordirection);
    Serial.print("\tspeed = ");
    Serial.println(motorspeed);
  }
#endif
}
