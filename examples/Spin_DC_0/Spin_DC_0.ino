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
//     Motor goes ^here^
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

void
setup()
{
	// empty
}

void
loop()
{
	L293.DCMotor_set(0, L293.dir_forward);
	for (int i = 0; i < 255; i++) {
		L293.DCMotor_set_speed(0, i);
		delay(1000 / 255);	// don't care about drift
	}
	for (int i = 254; i > 0; i--) {
		L293.DCMotor_set_speed(0, i);
		delay(1000 / 255);	// don't care about drift
	}
	L293.DCMotor_set(0, L293.dir_rewind);
	for (int i = 0; i < 256; i++) {
		L293.DCMotor_set_speed(0, i);
		delay(1000 / 255);	// don't care about drift
	}
	for (int i = 254; i > 0; i--) {
		L293.DCMotor_set_speed(0, i);
		delay(1000 / 255);	// don't care about drift
	}
}
