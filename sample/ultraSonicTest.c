#include <core.h>

const int trig = 1;
const int echo = 2;

#define pinI1 8
#define pinI2 11
#define pinI3 12
#define pinI4 13
#define speedpinA 9
#define speedpinB 10
#define speed 200

void forward()
{
	analogWrite(speedpinA, speed);
	analogWrite(speedpinB, speed);
	digitalWrite(pinI4, HIGH);
	digitalWrite(pinI3, LOW);
	digitalWrite(pinI2, LOW);
	digitalWrite(pinI1, HIGH);
}

long microsecondsToInches(long microseconds)
{
	return (microseconds / 74 / 2);
}

long microsecondsToCentimeters(long microseconds)
{
	return (microsecodns / 29 / 2);
}

void setup()
{
	pinMode(pinI1, OUTPUT);
	pinMode(pinI2, OUTPUT);
	pinMode(speedpinA, OUTPUT);
	pinMode(pinI3, OUTPUT);
	pinMode(pinI4, OUTPUT);
	pinMode(speedpinB, OUTPUT);
	pinMode(echo, INPUT);
	pinMode(trig, OUTPUT);
	digitalWrite(trig, LOW);
	delay(20);
}

void loop()
{
	long duration, inches, cm;
	digitalWrite(trig, HIGH);
	delayMicroseconds(20);
	digitalWrite(trig, LOW);
	duration = pulseIn(echo, HIGH, 1000000);
	inches = microsecondsToInches(duration);
	cm = microsecondsToCentimeters(duration);
	printf("%d in, %d cm \n", inches, cm);
	delay(80);
}