// Simple Motor Shield sketch
// -----------------------------------
//
// by Bodo Eggert <7eggert@gmx.de>, August 2016
//
// Open Source / Public Domain
//
// Inspired by a work from  arduino.cc user "Krodal".
// June 2012
// Open Source / Public Domain
//
// Using Arduino 1.16.10
//
// A simple sketch for the motor shield,
// without using the Adafruit library.
//
// The outputs can be used for DC-motors 
// (either full H-bridge or just On and Off), lights, 
// relays, solenoids, etc.
// But stepper motors can not be used !
// Servo motors can be used with the default Servo library.
//
// A maximum of 4 DC motors can be used with full-bridge,
// or a maximum of 8 normal outputs, or a combination.
// Two servo motors can always be used, they use the +5V 
// of the Arduino board, so the voltage regulator could 
// get hot.
//
// Tested with an Ebay clone with the Arduino Uno.
//
// Parts of the code are from an old Adafruit Motor Shield
// library, which was public domain at that time.
// This code is also public domain
//
// This simplified program is using the normal 
// Arduino library functions as much as possible.
//
// The motors will make a whistling sound, 
// due to the analogWrite() PWM frequency.
// The Adafruit library is specifically designed to avoid
// this, so use the Adafruit library for a better result.
//
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
//     middle        = GND
//     M2 on inside  = MOTOR2_A   (+)
//     M2 on outside = MOTOR2_B   (-) south
//
//   Connector X2:
//     M3 on outside = MOTOR3_B   (-) south
//     M3 on inside  = MOTOR3_A   (+)
//     middle        = GND
//     M4 on inside  = MOTOR4_B   (-)
//     M4 on outside = MOTOR4_A   (+) north
//
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
//
//
// Pin usage with the Motorshield
// ---------------------------------------
// Analog pins: not used at all
//     A0 ... A5 are still available
//     They all can also be used as digital pins.
//     Also I2C (A4=SDA and A5=SCL) can be used.
//     These pins have a breadboard area on the shield.
// Digital pins: used: 3,4,5,6,7,8,9,10,11,12
//     Pin 9 and 10 are only used for the servo motors.
//     Already in use: 0 (RX) and 1 (TX).
//     Unused: 2,13
//     Pin 2 has an soldering hole on the board, 
//           easy to connect a wire.
//     Pin 13 is also connected to the system led.
// I2C is possible, but SPI is not possible since 
// those pins are used.
//

#ifndef __L293_shield_included__
#define __L293_shield_included__
class t_L293 final {
      public:
	static const char dir_forward = 0x2;
	static const char dir_rewind = 0x1;
	static const char dir_stop = 0x0;

	/* bits in latch */
	static const char latches[8];

	/* pins for controlling motors */
	static const char PWM_Pin[4];
	static const char Servo_Pin[2];

	/* Low level functions:
	 * shiftWrite:  Write bit(s) to the latch
	 * shiftWrite8:Write full latch
	 */
	void shiftWrite(int n, int value);
	void shiftWrite(int n_high, int n_low, int values);
	void shiftWrite8(char newlatch);

	/* DC motors being connected to a pair of pins
	 *
	 * DCMotor_set_speed will actually pulse the outputs,
	 *  DCMotor_set_speed(0) will cause the outputs to be disconnected
	 */
	void DCMotor_set_speed(int n, int speed);

	void DCMotor_set(int n, int dir) {
		shiftWrite(latches[n * 2], latches[n * 2 + 1], dir);
	} void DCMotor_set(int n, int dir, int speed) {
		DCMotor_set(n, dir);
		DCMotor_set_speed(n, speed);
	}
	void DCMotor_set_speed_dir(int n, int speed) {
		if (speed < 0) {
			DCMotor_set(n, dir_rewind, -speed);
		} else {
			DCMotor_set(n, dir_forward, speed);
		}
	}
	// direction will be cleared
	void DCMotor_brake(int n) {
		DCMotor_set(n, dir_stop, 0xff);
	}
	// direction will NOT be cleared
	void DCMotor_release(int n) {
		// DCMotor_set_speed(n, 0);
		digitalWrite(PWM_Pin[n], 0);
	}

	/* DC motors being connected to one pin and GND
	 *
	 * DC1Motor_set_pairspeed will actually pulse the outputs,
	 *  DC1Motor_set_pairspeed(0) will cause the outputs to be disconnected
	 */
	void DC1Motor_set_pair_speed(int n, int speed) {
		DCMotor_set_speed(n >> 1, speed);
	}
	void DC1Motor_set(int n, int on_off) {
		shiftWrite(latches[n], on_off);
	}
	void DC1Motor_set_all_speed(int speed) {
		for (int n = 0; n < 4; n++)
			DCMotor_set_speed(n, speed);
	}
	// enable/disable will not change the status,
	// re-enabling or setting the speed will
	// use the old values.
	void DC1Motor_enable_all() {
		digitalWrite(PWM_Pin[0], 1);
		digitalWrite(PWM_Pin[1], 1);
		digitalWrite(PWM_Pin[2], 1);
		digitalWrite(PWM_Pin[3], 1);
	}
	void DC1Motor_disable_all() {
		digitalWrite(PWM_Pin[0], 0);
		digitalWrite(PWM_Pin[1], 0);
		digitalWrite(PWM_Pin[2], 0);
		digitalWrite(PWM_Pin[3], 0);
	}
	void DC1Motor_enable_all(int enabled) {
		digitalWrite(PWM_Pin[0], enabled);
		digitalWrite(PWM_Pin[1], enabled);
		digitalWrite(PWM_Pin[2], enabled);
		digitalWrite(PWM_Pin[3], enabled);
	}

      private:
	/* pins used for communication with latch */
	static const char IC74HC595N_LATCH = 12; // aka. STCP
	static const char IC74HC595N_CLK = 4;    // aka. SHCP
	static const char IC74HC595N_ENABLE = 7; // aka. ~OE
	static const char IC74HC595N_DATA = 8;   // aka. DS

	static char latch_copy;
      public:			// but don't use it, please
	t_L293();
} L293;

const char
t_L293::latches[8] = { 2, 3, 1, 4, 5, 7, 0, 6 };

const char
t_L293::PWM_Pin[4] = { 11, 3, 6, 5 };

const char
t_L293::Servo_Pin[2] = { 10, 9 };

char
    t_L293::latch_copy = 0;

t_L293::t_L293()
{
	pinMode(IC74HC595N_LATCH, OUTPUT);
	pinMode(IC74HC595N_ENABLE, OUTPUT);
	pinMode(IC74HC595N_DATA, OUTPUT);
	pinMode(IC74HC595N_CLK, OUTPUT);

	// Set pins for shift register to default value (low);
	digitalWrite(IC74HC595N_DATA, LOW);
	digitalWrite(IC74HC595N_LATCH, LOW);
	digitalWrite(IC74HC595N_CLK, LOW);
	// Enable the shift register, set Enable pin Low.
	digitalWrite(IC74HC595N_ENABLE, LOW);

	// avoid variables here, so we don't unnecessarily include the variable arrays
	pinMode(PWM_Pin[0], OUTPUT);
	pinMode(PWM_Pin[1], OUTPUT);
	pinMode(PWM_Pin[2], OUTPUT);
	pinMode(PWM_Pin[3], OUTPUT);
	DC1Motor_disable_all();	// disconnect ports
	// the digitalWrite above will cause a slight delay between setting
	// IC74HC595N_* to low and clearing the latches
	shiftWrite8(0);
	// now all connectors will connect to ground on
	// DCMotor_set_speed()
}

void
t_L293::shiftWrite(int n, int value)
{
	char new_latch_copy = latch_copy;
	bitWrite(new_latch_copy, n, value & 1);
	if (latch_copy != new_latch_copy)
		shiftWrite8(new_latch_copy);
}

void
t_L293::shiftWrite(int n_high, int n_low, int values)
{
	char new_latch_copy = latch_copy;
	bitWrite(new_latch_copy, n_high, (values >> 1) & 1);
	bitWrite(new_latch_copy, n_low, (values >> 0) & 1);
	if (latch_copy != new_latch_copy)
		shiftWrite8(new_latch_copy);
}

void
t_L293::shiftWrite8(char newlatch)
{
	// Use the default Arduino 'shiftOut()' function to
	// shift the bits with the IC74HC595N_CLK as clock pulse.
	// The 74HC595 shiftregister wants the MSB first.
	// After that, generate a latch pulse with IC74HC595N_LATCH.
	latch_copy = newlatch;
	shiftOut(IC74HC595N_DATA, IC74HC595N_CLK, MSBFIRST, newlatch);
	delayMicroseconds(5);	// For safety, not really needed.
	digitalWrite(IC74HC595N_LATCH, HIGH);
	delayMicroseconds(5);	// For safety, not really needed.
	digitalWrite(IC74HC595N_LATCH, LOW);
}

void
t_L293::DCMotor_set_speed(int n, int speed)
{
	if (speed & ~0xff)	// not in 0..255
		return;
	if (n & ~0x03)		// not in 0..3
		return;
	analogWrite(PWM_Pin[n], speed);
}
#endif
