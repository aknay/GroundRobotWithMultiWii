
#ifndef Receiver_h
#define Receiver_h

#include "Arduino.h"

class Receiver
{
	public:
	Receiver(int pinChannel);
	int getRcTiming(int minValue, int maxValue); 
	private:
	int m_pinChannel;
};

#endif