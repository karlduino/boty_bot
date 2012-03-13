/***********************************************************************/
/* definitions and variables for the Servo                             */
/***********************************************************************/
Servo ping_servo;
unsigned long lastRandomServoMove = 0;
#define SERVO_CORRECTION 5
#define SERVO_ANGLE_MIN 15
#define SERVO_ANGLE_MAX 165
/***********************************************************************/

void move_servo_slowly(Servo servo, int new_angle, int n_steps, int delay_betw_steps);
void random_servo_angle(Servo servo);

int scan_and_ping(Servo servo, int left, int right);
