Arduino code for my first robot
Karlduino
March 2012

Apple IR remote using code from hifiduino.wordpress.com

Setup: 
    - Adafruit motor shield
      DC motors M1: front left
                M2: back  left
                M3: back  right
                M4: front right
      Servo (attached to Ping sensor): servo 1 = pin 10

    - Ping sensor: digital pin 19 (aka analog pin 5)

    - IR sensor: digital pin 2

    - seven segment LED display: digital pins 14 and 15 (aka analog pins 0 and 1)
      pin 14 is the reset; pin 15 transmits ping distance

    - led bulb: digital pin 13
