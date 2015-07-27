
#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

class Motor
{
	public:
	Motor(int pinPWMA, int pinAIN2, int pinAIN1, int pinSTBY, int pinBIN1, int pinBIN2, int pinPWMB);
	void moveForward(uint8_t leftMotorSpeed, uint8_t rightMotorSpeed);
	void moveBackward(uint8_t leftMotorSpeed, uint8_t rightMotorSpeed);
	void moveMotors(int pitchChannel, int rollChannel);
	void stopMotors();
	private:
	int m_pinPWMA, m_pinAIN2, m_pinAIN1, m_pinSTBY, m_pinBIN1, m_pinBIN2, m_pinPWMB;
};

#endif
