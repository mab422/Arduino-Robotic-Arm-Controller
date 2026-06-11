/*
 * Robot Arm Controller for Summer STEM Camp 2026
 * 
 * Joystick 1 (powered by pin 8):
 *   VRy (A0) = Turret (rotate base)
 *   VRx (A1) = Lift Arm (up/down)
 *
 * Joystick 2 (powered by pin 7):
 *   VRy (A2) = Gripper (open/close)
 *   VRx (A3) = Reach Arm (extend/retract)
 *
 * Servos on PCA9685:
 *   Channel 0 = Turret
 *   Channel 1 = Lift Arm
 *   Channel 2 = Reach Arm
 *   Channel 3 = Gripper
 */


#include <Adafruit_PWMServoDriver.h> // library that talks to the servo driver board


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40); // create a connection to the servo driver board


const int SERVO_MIN = 150;  //pulse 150 = 0 degrees
const int SERVO_MAX = 600;  //pulse 600 = 180 degrees

// Settings for each joystick axis (A0, A1, A2, A3)
// Each column is one axis — change these to adjust behavior
//                              A0       A1        A2       A3
const int servo[]       = {     0,       1,        3,       2     };  // which servo it controls (see above)
const int minAngle[]    = {     2,       0,        45,       2     };  // smallest safe angle
const int maxAngle[]    = {   160,      90,       145,     160     };  // largest safe angle
const float speed[]     = {   0.6,     0.6,      0.3,     0.6    };  // how fast it moves
const int direction[]   = {     1,      -1,        1,       1     };  // 1 = normal, -1 = reversed

// Where each servo starts when you power on
//                          Turret  LiftArm  ReachArm  Gripper
float angle[] =           {  81.0,    90.0,    100.0,     45.0  };


const int DEADZONE = 40; // ignore small joystick movements (prevents drift when not touching it)

void setup() {
 
  pinMode(8, OUTPUT);  //declare digital pin 8 as output
  pinMode(7, OUTPUT);  //declare digital pin 7 as outptut
  digitalWrite(8, HIGH);  //turn on power for joystick 1
  digitalWrite(7, HIGH);  //turn on power for joystick 2

  // start the servo driver board
  pwm.begin();
  pwm.setPWMFreq(50);
  delay(10);

  // move all 4 servos to their starting positions
  for (int i = 0; i < 4; i++) {
    // convert degrees to a pulse number and send it to the servo
    pwm.setPWM(i, 0, (int)(SERVO_MIN + (angle[i] / 180.0) * (SERVO_MAX - SERVO_MIN) + 0.5)); //convert motor angles to pulses
  }
}

void loop() {
 
  int pins[] = { A0, A1, A2, A3 }; //analog pins connected to joysticks

  // check each joystick axis
  for (int i = 0; i < 4; i++) {
    
    int reading = analogRead(pins[i]); // read the joystick position (0 to 1023, 512 = center)

    
    int offset = reading - 512; // how far the joystick is from center

    
    if (abs(offset) < DEADZONE) continue; // ff the joystick is barely moved, skip it

  
    int ch = servo[i];  //map analog pin (joy axis) onto servo

    
    float movement = (float)offset / 512.0 * direction[i] * speed[i]; // calculate how much to move the servo
    angle[ch] += movement;

  
    if (angle[ch] < minAngle[ch]) angle[ch] = minAngle[ch];  //safety limits (min)
    if (angle[ch] > maxAngle[ch]) angle[ch] = maxAngle[ch];  //safety limit (max)

   
    pwm.setPWM(ch, 0, (int)(SERVO_MIN + (angle[ch] / 180.0) * (SERVO_MAX - SERVO_MIN) + 0.5));  //movement pulse to motor
  }

  
  delay(15);
}
