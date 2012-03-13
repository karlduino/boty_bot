/***********************************************************************/
/* definitions and variables for DC motors                             */
/***********************************************************************/
AF_DCMotor frontleft(1, MOTOR12_1KHZ);
AF_DCMotor backleft(2, MOTOR12_1KHZ);
AF_DCMotor backright(3, MOTOR34_1KHZ);
AF_DCMotor frontright(4, MOTOR34_1KHZ);

int motorspeed=0;
int motordirection=RELEASE;
#define MIN_SPEED 100
#define MAX_SPEED 255
#define SPEED_INCREMENT 10
#define TURN_SPEED 210
#define TURN_TIME 300
#define DELAY_AFTER_STOP 1000
#define RIGHT_SPEED_CORRECTION_FORWARD 10
#define RIGHT_SPEED_CORRECTION_BACKWARD 20

#ifdef DEBUG
long last_motor_speed_report = 0;
#endif


void set_all_speeds(int speed, int direction);
void run_all_motors(int direction);
void full_stop(void);
void spin(int direction, int time, int speed);


