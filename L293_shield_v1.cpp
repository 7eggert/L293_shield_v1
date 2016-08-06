#include <Arduino.h>
#include "L293_shield_v1.h"

 t_L293 L293;

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
	char
	    new_latch_copy = latch_copy;
	bitWrite(new_latch_copy, n, value & 1);
	if (latch_copy != new_latch_copy)
		shiftWrite8(new_latch_copy);
}

void
 t_L293::shiftWrite(int n_high, int n_low, int values)
{
	char
	    new_latch_copy = latch_copy;
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
