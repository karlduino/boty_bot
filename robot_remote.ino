/***********************************************************************/
/* Code for Apple IR remote from hifiduino.wordpress.com               */
/***********************************************************************/
/*
   The following function returns the code from the Apple Aluminum
   remote control.  [Modified to also handle the little white apple
   remote.]  The Apple remote is based on the NEC infrared remote
   protocol. Of the 32 bits (4 bytes) coded in the protocol, only the
   third byte corresponds to the keys. The function also handles
   errors due to noise (returns 255) and the repeat code (returns
   zero)

 The Apple remote returns the following codes:
 
   Up key:     238 135 011 089
   Down key:   238 135 013 089
   Left key:   238 135 008 089
   Right key:  238 135 007 089
   Center key: 238 135 093 089 followed by 238 135 004 089 (See blog for why there are two codes)
   Menu key:   238 135 002 089
   Play key:   238 135 094 089 followed by 238 135 004 089 [also gives right key code]
   
Older (white) remote:
   Play:   5
   Right:  6
   Left:   9
   Up:    10
   Down:  12
   Menu:   3
 */

int getIRkey(void) 
{
  c1=0;
  c2=0;
  c3=0;
  c4=0;
  duration=1;
  while((duration=pulseIn(REMOTEPIN, HIGH, 15000)) < 2000 && duration!=0)
  {
    // Wait for start pulse
  }
  if (duration == 0)         // This is an error no start or end of pulse
    return(255);             // Use 255 as Error

  else if (duration<3000)    // This is the repeat
    return (0);              // Use zero as the repeat code

  else if (duration<5000){   // This is the command get the 4 byte
    mask = 1;				    
    for (int i = 0; i < 8; i++){	             // get 8 bits
      if(pulseIn(REMOTEPIN, HIGH, 3000)>1000)     // If "1" pulse
        c1 |= mask;			             // Put the "1" in position
      mask <<= 1;				     // shift mask to next bit
    }
    mask = 1;				    
    for (int i = 0; i < 8; i++){	             // get 8 bits
      if(pulseIn(REMOTEPIN, HIGH, 3000)>1000)     // If "1" pulse
        c2 |= mask;			             // Put the "1" in position
      mask <<= 1;				     // shift mask to next bit
    }
    mask = 1;				    
    for (int i = 0; i < 8; i++){	             // get 8 bits
      if(pulseIn(REMOTEPIN, HIGH, 3000)>1000)     // If "1" pulse
        c3 |= mask;			             // Put the "1" in position
      mask <<= 1;				     // shift mask to next bit
    }
    mask = 1;				    
    for (int i = 0; i < 8; i++){	             // get 8 bits
      if(pulseIn(REMOTEPIN, HIGH, 3000)>1000)     // If "1" pulse
        c4 |= mask;			             // Put the "1" in position
      mask <<= 1;				     // shift mask to next bit
    }
    return(c3);
  }
  return(255); // error code
}

// interpret IR key, dealing with repeats
// (here, save only repeat UPs and DOWNs)
int handleIRkey(int IRkey, int lastIRkey)
{
  if(IRkey == 0) { // repeat code
    interpretedIRkey = interpretIRkey(previousIRkey, lastIRkey); // interpret previous key
    //    if(interpretedIRkey == UP || interpretedIRkey == DOWN)
      // take repeat UP or DOWN but nothing else
      IRkey = previousIRkey;
      //    }
      //    else { // ignore any other repeat
      //      interpretedIRkey = 0;
      //    }
  }   
  else {
    interpretedIRkey = interpretIRkey(IRkey, lastIRkey);
    if(interpretedIRkey != 0) previousIRkey = IRkey; // save as previous only if non-zero
  }

  return(interpretedIRkey);
}



// simpler interpretation of the IRkey press for both apple remotes
int interpretIRkey(int IRkey, int lastIRkey)
{
#ifdef DEBUG
  if(IRkey != 0 && IRkey != 255) {
    Serial.print("raw IR key: ");
    Serial.println(IRkey);
    Serial.print("last IR key: ");
    Serial.println(lastIRkey);
  }
#endif
    switch(IRkey){
    // case 0 and 255 are "valid" cases from the code, but we do nothing in this switch statement
    case 11:  // 11 is up key on aluminum remote
    case 10:  // 10 is up key on old remote
      return(UP);
    case 7:   // 7 is right key on aluminum remote
      if(lastIRkey == 10 || lastIRkey == 12 || lastIRkey == 5) { // ignore 7 if it follows 10 or 12 (old remote)
#ifdef DEBUG
	Serial.println("Ignoring 7 IR key");
#endif
	return(255);
      }
    case 6:   // 6 is right key on old remote
      return(RIGHT);
    case 13:  // 13 is down key on aluminum remote
    case 12:  // 12 is down key on old remote
      return(DOWN);
    case 8:   // 8 is left key on aluminum remote
    case 9:   // 9 is left key on old remote
      return(LEFT);
    case 4:  // 4 is the center key on aluminum remote
    case 5:  // 5 is the center key on old remote
    case 93:  // 93 is center key on aluminum remote
    case 94:  // 94 is play/pause key on aluminum remote
      return(PLAY);
    case 2:  // 2 is menu key on aluminum remote
    case 3:  // 3 is menu key on old remote
      return(MENU);
    case 0:   // ignore 0
    case 255: // ignore 255
    default: 
      return(0); // to be ignored
    }
}

// print codes
void print_interpretedIRkey(int interpretedIRkey)
{
  switch(interpretedIRkey) {
    case UP: 
      Serial.println("Up");
      break;
    case RIGHT: 
      Serial.println("Right");
      break;
    case DOWN: 
      Serial.println("Down");
      break;
    case LEFT: 
      Serial.println("Left");
      break;
    case PLAY: 
      Serial.println("Play");
      break;
    case MENU: 
      Serial.println("Menu");
      break;
  default: 
    Serial.print("Different button: ");
    Serial.println(interpretedIRkey);
  }
}
