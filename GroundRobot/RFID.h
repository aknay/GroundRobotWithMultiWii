
#ifndef RFID_h
#define RFID_h

#include "Arduino.h"

class RFID
{
	public:
	RFID(int pinReset);
	void scanTag();
	bool isReadyToReadData();
	String getTagID();
	private:
	int m_pinReset;
    String m_ScannedTag;
	bool m_isReadyToSendOut;
};

#endif