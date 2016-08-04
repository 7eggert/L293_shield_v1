// Simple Motor Shield sketch
// -----------------------------------
//
// Spin a DC motor connected to M1 (Motor number 0) up and down into reverse
// 
// by Bodo Eggert <7eggert@gmx.de>
// see library file for additionla remarks on authorship and connectors
// 
// August 2016
// Open Source / Public Domain
//
// Using Arduino 1.6.10
//
//
// Connector usage
// ---------------
// The order is different than what you would expect.
// If the Arduino (Uno) board is held with the USB
// connector to the left, the positive (A) side is 
// at the top (north), and the negative (B) side is 
// the bottom (south) for both headers.
//
//   Connector X1:
//     M1 on outside = MOTOR1_A   (+) north
//     M1 on inside  = MOTOR1_B   (-)
//
//   Motor 0 goes ^here^
//
//     M2 on inside  = MOTOR2_A   (+)
//     M2 on outside = MOTOR2_B   (-) south
//
//   Motor 1 goes ^here^
//
//   Connector X2:
//     M3 on outside = MOTOR3_B   (-) south
//     M3 on inside  = MOTOR3_A   (+)
//
//   Motor 2 goes ^here^
//
//     M4 on inside  = MOTOR4_B   (-)
//     M4 on outside = MOTOR4_A   (+) north
//
//   Motor 3 goes ^here^
//
//         -------------------------------
//         | -+s                         |
//         | -+s                         |
//    M1 A |                             | M4 A
//    M1 B |                             | M4 B
//    GND  |                             | GND
//    M2 A |                             | M3 A
//    M2 B |                             | M3 B
//         |                       ..... |
//         -------------------------------
//                + - 
//

#include <L293_shield_v1.h>

void setup() {
  digitalWrite(13, 1);
  L293.DCMotor_set(0, L293.dir_forward);
  L293.DCMotor_set(1, L293.dir_forward);
  for (int i=1; i<256; i++) {
    L293.DCMotor_set_speed(0,i);
    delay(1000/255); // don't care about drift
  }
  digitalWrite(13, 0);
  // motor 0 is spinning, 1 is at rest
  for (int i=1; i<256; i++) {
    L293.DCMotor_set_speed(0,256-i);
    L293.DCMotor_set_speed(1,i);
    delay(1000/255); // don't care about drift
  }
  digitalWrite(13, 1);
  // motor 0 is at rest, 1 is spinning
  L293.DCMotor_set(0, L293.dir_rewind);
  // L293.DCMotor_set(2, L293.dir_forward); still in effect
  L293.DCMotor_set(2, L293.dir_forward);
  for (int i=1; i<256; i++) {
    L293.DCMotor_set_speed(0,i);
    L293.DCMotor_set_speed(1,256-i);
    L293.DCMotor_set_speed(2,i);
    delay(1000/255); // don't care about drift
  }
  // motor 0 in reverse (full speed), motor 2 in forward (full speed)
  // others at rest
}

void slide(const int m0)
{
  // motor m0 in reverse (full speed), motor m2 in forward (full speed)
  // others at rest
  const int m1 = (m0+1) & 3;
  const int m2 = (m0+2) & 3;
  const int m3 = (m0+2) & 3;
  digitalWrite(13, m0 & 1);
  L293.DCMotor_set(m1, L293.dir_rewind);
  L293.DCMotor_set(m3, L293.dir_forward);
  for (int i=1; i<256; i++) {
    L293.DCMotor_set_speed(m0,256-i);
    L293.DCMotor_set_speed(m2,256-i);
    L293.DCMotor_set_speed(m1,i);
    L293.DCMotor_set_speed(m3,i);
    delay(1000/255); // don't care about drift
  }
  // motor m1 in reverse (full speed), motor m3 in forward (full speed)
  // others at rest
}

void loop()
{
  for (int i=0; i<4; i++)
    slide(i);
}

