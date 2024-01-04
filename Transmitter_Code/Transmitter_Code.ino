// 4 Channel Transmitter

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeOut = 0xE9E8F0F0E1LL;   //IMPORTANT: The same as in the receiver 0xE9E8F0F0E1LL 
RF24 radio(7, 8); // select CE,CSN pin | CE ve CSN pinlerin seçimi

struct Signal {
byte throttle;
byte pitch;
byte roll;
byte yaw;
};

Signal data;

void ResetData() 
{
data.throttle = 127; // Motor Stop (254/2=127) (Signal lost position)
data.pitch = 127; // Center (Signal lost position )
data.roll = 127; // Center  (Signal lost position )
data.yaw = 127; // Center   (Signal lost position )
}

void setup()
{
//Start everything up


radio.begin();
radio.openWritingPipe(pipeOut);
radio.stopListening(); //start the radio comunication for Transmitter 
ResetData();
}

 //Joystick center and its borders 

int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse)
{
val = constrain(val, lower, upper);
if ( val < middle )
val = map(val, lower, middle, 0, 128);
else
val = map(val, middle, upper, 128, 255);
return ( reverse ? 255 - val : val );
}

void loop()
{
// Control Stick Calibration 

data.throttle = mapJoystickValues( analogRead(A0), 530, 530, 1020, false );
data.roll = mapJoystickValues( analogRead(A1), 12, 524, 1020, false );      // "true" or "false" for servo direction 
data.pitch = mapJoystickValues( analogRead(A2), 12, 524, 1020, false );     // "true" or "false" for servo direction 
data.yaw = mapJoystickValues( analogRead(A3), 12, 524, 1020, true );       // "true" or "false" for servo direction 

radio.write(&data, sizeof(Signal));
}
