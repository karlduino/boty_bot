/***********************************************************************/
/* DC motor functions                                                  */
/***********************************************************************/
void set_all_speeds(int speed, int direction)
{
  int right_speed, left_speed;
  switch(direction) {
  case FORWARD:
    right_speed = speed + RIGHT_SPEED_CORRECTION_FORWARD;
    right_speed = constrain(right_speed, MIN_SPEED, MAX_SPEED);
    left_speed = right_speed - RIGHT_SPEED_CORRECTION_FORWARD;
    break;
  case BACKWARD:
    left_speed = speed + RIGHT_SPEED_CORRECTION_BACKWARD;
    left_speed = constrain(left_speed, MIN_SPEED, MAX_SPEED);
    right_speed = left_speed - RIGHT_SPEED_CORRECTION_BACKWARD;
  default:
    left_speed = right_speed = speed;
  }
  
  frontleft.setSpeed(left_speed);
  frontright.setSpeed(right_speed);
  backleft.setSpeed(left_speed);
  backright.setSpeed(right_speed);
}

void run_all_motors(int direction)
{
  frontleft.run(direction);
  frontright.run(direction);
  backleft.run(direction);
  backright.run(direction);
}

void full_stop(void)
{
  run_all_motors(RELEASE);
}

// direction=2 : RIGHT
// direction=4 : LEFT
void spin(int direction, int time, int speed) 
{
  int forback;
  switch(direction) {
  case RIGHT: 
    forback = 1;
    break;
  case LEFT:
    forback = 2;
    break;
  default:
    Serial.print("spin() called with invalid direction = ");
    Serial.println(direction);
    return;
  }
    
  set_all_speeds(speed, direction);
  frontleft.run(forback);
  backleft.run(forback);
  frontright.run(3-forback);
  backright.run(3-forback);

  delay(time);

  run_all_motors(RELEASE);
}
