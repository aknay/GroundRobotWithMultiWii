#include "Arduino.h"
#include "RFID.h"

RFID::RFID(int pinReset)
{
	m_pinReset = pinReset;
	pinMode(m_pinReset, OUTPUT);
}

void RFID::scanTag(){
	char tagString[13]={0};
	int index = 0;
	bool reading = false;
	bool okToPrintOut = false;
	bool isScannedSuccessful = false;
	
	m_isReadyToSendOut = false;
	
	while(Serial.available()){
		int readByte =  Serial.read();
		if(readByte == 2) reading = true; //begining of tag
		if(readByte == 3) reading = false; //end of tag

		if(reading && readByte != 2 && readByte != 10 && readByte != 13){
			//store the tag
			tagString[index] = readByte;
			index ++;
			//Serial.print(readByte);
			okToPrintOut = true;
	}
	
}

if (okToPrintOut == true && index == 12){
//Serial.println(tagString);
m_isReadyToSendOut = true;
 m_ScannedTag = tagString;
}
 digitalWrite(m_pinReset, LOW);
 delay(100);
 digitalWrite(m_pinReset, HIGH);
 delay(100);

// return tagString;
}

bool RFID::isReadyToReadData(){
	return m_isReadyToSendOut; 
}

String RFID::getTagID(){
return m_ScannedTag;
}