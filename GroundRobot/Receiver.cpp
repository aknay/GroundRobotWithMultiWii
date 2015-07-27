#include "Arduino.h"
#include "Receiver.h"

Receiver::Receiver(int pinChannel)
{
	m_pinChannel = pinChannel;
	pinMode(m_pinChannel, INPUT);

}
int Receiver::getRcTiming(int minValue, int maxValue)
{
	int result;
	result = pulseIn(m_pinChannel, HIGH, 25000); 
	result =  map(result,1000,2000,minValue, maxValue);
	result =  constrain(result,minValue,maxValue);
	
	 if ((result >= 0) && (result < 10)) {
		 result = 5;
	 }
	 if ((result <= 0) && (result > -10)) {
		 result = -5;
	 }
	return result;
    
	
	
}


