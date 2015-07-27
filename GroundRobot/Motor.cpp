#include "Arduino.h"
#include "Motor.h"

Motor::Motor(int pinPWMA, int pinAIN2, int pinAIN1, int pinSTBY, int pinBIN1, int pinBIN2, int pinPWMB)
{
	m_pinPWMA = pinPWMA;
	m_pinAIN2 = pinAIN2;
	m_pinAIN1 = pinAIN1;
	
	m_pinSTBY = pinSTBY;
	
	m_pinBIN1 = pinBIN1;
	m_pinBIN2 = pinBIN2;
	m_pinPWMB = pinPWMB;
	
	pinMode(pinPWMA, OUTPUT);
	pinMode(pinAIN1, OUTPUT);
	pinMode(pinAIN2, OUTPUT);
	
	pinMode(pinPWMB, OUTPUT);
	pinMode(pinBIN1, OUTPUT);
	pinMode(pinBIN2, OUTPUT);
	
	pinMode(pinSTBY, OUTPUT);
	
}

void Motor::moveForward(uint8_t leftMotorSpeed, uint8_t rightMotorSpeed){
	 digitalWrite(m_pinSTBY, HIGH);
	 
	 digitalWrite(m_pinBIN1, HIGH);
	 digitalWrite(m_pinBIN2, LOW);
	 analogWrite(m_pinPWMB, rightMotorSpeed);
	
	 digitalWrite(m_pinAIN1, LOW);
	 digitalWrite(m_pinAIN2, HIGH);
	 analogWrite(m_pinPWMA, leftMotorSpeed);
}

void Motor::moveBackward(uint8_t leftMotorSpeed, uint8_t rightMotorSpeed){
	digitalWrite(m_pinSTBY, HIGH);
	
	digitalWrite(m_pinBIN1, LOW);
	digitalWrite(m_pinBIN2, HIGH);
	analogWrite(m_pinPWMB, rightMotorSpeed);
	
	digitalWrite(m_pinAIN1, HIGH);
	digitalWrite(m_pinAIN2, LOW);
	analogWrite(m_pinPWMA, leftMotorSpeed);
}


void Motor::moveMotors(int pitchChannel, int rollChannel){
	uint8_t leftMotorSpeed, rightMotorSpeed;
	
	 if ((rollChannel > 0) && (pitchChannel >0))
	 {
		 

		leftMotorSpeed = constrain (pitchChannel + rollChannel,0,250);
		rightMotorSpeed = constrain (pitchChannel - rollChannel,0,250);
		moveForward(leftMotorSpeed,rightMotorSpeed);
	
	 }
	 else if ((rollChannel < 0) && (pitchChannel >0))
	 {
		 //moveForward(pitchChannel - abs(rollChannel),pitchChannel + abs(rollChannel));
		 
		 leftMotorSpeed = constrain (pitchChannel - abs(rollChannel),0,250);
		 rightMotorSpeed = constrain (pitchChannel + abs(rollChannel),0,250);
		 moveForward(leftMotorSpeed,rightMotorSpeed);
	 }
	 else if ((pitchChannel < 0) && (rollChannel >0))
	 {
		 leftMotorSpeed = constrain (abs(pitchChannel) + rollChannel,0,250);
		 rightMotorSpeed = constrain (abs(pitchChannel) - rollChannel,0,250);
		 moveBackward(leftMotorSpeed,rightMotorSpeed);
	 }
	 else if ((pitchChannel < 0) && (rollChannel <0))
	 {

		 leftMotorSpeed = constrain (abs(pitchChannel) - abs(rollChannel),0,250);
		 rightMotorSpeed = constrain (abs(pitchChannel) + abs(rollChannel),0,250);
		 moveBackward(leftMotorSpeed,rightMotorSpeed);
	 }
	 else
	 {
		 stopMotors();
	 }
	
	
	
	
	
}

void Motor::stopMotors(){
	 digitalWrite(m_pinSTBY, LOW);
	 analogWrite(m_pinPWMB, 5);
	 analogWrite(m_pinPWMA,5);
} 