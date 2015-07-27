/*
 * GroundRobot.ino
 *
 * Created: 3/6/2015 10:56:35 AM
 * Author: Nay Aung Kyaw
 */ 
#include "Motor.h"
#include "Receiver.h"
#include "RFID.h"
#include "PinTranslation.h"

Motor motor(D3,YAW,D9,D10,D11,D12,PIT); //PWMA, AIN2, AIN1, STBY, BIN1, BIN2, PWMB
Receiver pitch(ROL); //Channel 2 OR 3
Receiver roll(THR); // Channel 4
RFID rfid(AX1); //RST PIN

void setup()
{
Serial.begin(9600);
Serial.println("Setup Done.");
}

void loop()
{
motor.moveMotors(pitch.getRcTiming(-255,255),roll.getRcTiming(-64,64));
rfid.scanTag();
if (rfid.isReadyToReadData()){
	Serial.print("GBot:4 Station No:");
	Serial.println(rfid.getTagID());
}
}
