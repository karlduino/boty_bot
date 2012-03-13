/***********************************************************************/
/* Code for Apple IR remote from hifiduino.wordpress.com               */
/***********************************************************************/
#define REMOTEPIN 2      // Pin for IR receiver (remote control)
int IRkey;               // The unique code (Byte 3) of the remote key
int lastIRkey=255;       // used to ignore 7 that follows 10 and 12 in old remote
int previousIRkey=0;     // The previous code (used for repeat)
int interpretedIRkey;    // Interpretation of IRkey
#define UP    1
#define RIGHT 2
#define DOWN  3
#define LEFT  4
#define PLAY  5
#define MENU  6
const int led13Pin = 13;
int led13State = 1;

// The following variables for the remote control feature
int duration;         // Duration of the IR pulse
int mask;             // Used to decode the remote signals 
int c1;               // Byte 1 of the 32-bit remote command code
int c2;               // Byte 2 of the 32-bit remote command code
int c3;               // Byte 3 of the 32-bit remote command code
int c4;               // Byte 4 of the 32-bit remote command code

/*
The following function returns the code from the Apple Aluminum remote control. The Apple remote is
 based on the NEC infrared remote protocol. Of the 32 bits (4 bytes) coded in the protocol, only the
 third byte corresponds to the keys. The function also handles errors due to noise (returns 255) and
 the repeat code (returs zero)
*/
int getIRkey(void);

// interpret IR key, dealing with repeats
// (here, save only repeat UPs and DOWNs)
int handleIRkey(int IRkey);

// simpler interpretation of the IRkey press for both apple remotes
int interpretIRkey(int IRkey);

// print codes
void print_interpretedIRkey(int interpretedIRkey);
