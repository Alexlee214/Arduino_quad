#include <RF24.h>

const byte LY = A1;
const byte LX = A0;
const byte LClick = 2;

const byte RY = A3;
const byte RX = A2;
const byte RClick = 3;

//joystick has value of around 520 when in the center position
//range is from 0 to 1023
short LXCenter;
short LYCenter;
short RXCenter;
short RYCenter;

//allow for a bit of error from the joytick
const byte errorTolerance = 2;

RF24 radio(7, 8); // CE, CSN


void setup() {
  setupJoystick();
  radioTransmitSetup();
  Serial.begin(9600);
}
//----------------------------------------------------------------------------------------------




void loop() {
  short readValue = 0;
  short sendValue = 0;
  short message[6];
  
  //when the left joystick is adjusted up
  if(analogRead(LY) - LYCenter >= errorTolerance){
      readValue = analogRead(LY) - LYCenter;
      sendValue = mapAcceleration(readValue); 
  //when the left joystick is adjusted down    
  }else if(LYCenter - analogRead(LY) >= errorTolerance){
      readValue = LYCenter - analogRead(LY);
      sendValue = -(mapAcceleration(readValue));
  }
  //radio.write(&LYValue, sizeof(LYValue));
  message[0] = sendValue;

  sendValue = 0;

  //when the left joystick is adjusted right
  if(analogRead(LX) - LXCenter >= errorTolerance){
      readValue = analogRead(LX) - LXCenter;
      sendValue = mapSpeed(readValue);
  //when the left joystick is adjusted left   
  }else if(LXCenter - analogRead(LX) >= errorTolerance){
      readValue = LXCenter - analogRead(LX);
      sendValue = -(mapSpeed(readValue));
  }
  message[1] = sendValue;

  sendValue = 0;

  //when the right joystick is adjusted up
  if(analogRead(RY) - RYCenter >= errorTolerance){
      readValue = analogRead(RY) - RYCenter;
      sendValue = mapSpeed(readValue);
  //when the right joystick is adjusted down  
  }else if(RYCenter - analogRead(RY) >= errorTolerance){
      readValue = RYCenter - analogRead(RY);
      sendValue = -(mapSpeed(readValue));
  }
  message[2] = sendValue;
    Serial.print(analogRead(LY));
  Serial.print("  ");
  Serial.println(analogRead(LX));
  Serial.print(analogRead(RY));
  Serial.print("  ");
  Serial.println(analogRead(RX));

  sendValue = 0;

  //when the right joystick is adjusted up
  if(analogRead(RX) - RXCenter >= errorTolerance){
      readValue = analogRead(RX) - RXCenter;
      sendValue = mapSpeed(readValue);
  //when the right joystick is adjusted down  
  }else if(RXCenter - analogRead(RX) >= errorTolerance){
      readValue = RXCenter - analogRead(RX);
      sendValue = -(mapSpeed(readValue));
  }
  message[3] = sendValue;

  sendValue = 0;
  
  
  if(digitalRead(LClick) == LOW) sendValue = 1;
  message[4] = sendValue;

  sendValue = 0;

  if(digitalRead(RClick) == LOW) sendValue = 1;
  message[5] = sendValue;

  sendValue = 0;

  radio.write(&message, sizeof(message));

  delay(100);
}
//---------------------------------------------------------------------------------------------


//Maps the value reading from the joystick to actual acceleration
//param: reading from the joystick reading, is directional, thus, value range from 0 to 550
//return: an acceleration value from 1 to 15 (change value to modify acceleration)
short mapAcceleration(int joyReading){
  return (byte)map(joyReading, 0, 520, 1, 15);
}
//---------------------------------------------------------------------------------------------



//Maps the value reading from the joystick to actual speed to pitch/roll/yaw
//param: reading from the joystick reading, is directional, thus, value range from 0 to 550
//return: a speed value from 1 to 50 (change value to modify speed of motion)
short mapSpeed(int joyReading){
  return (byte)map(joyReading, 0, 520, 1, 50);
}
//---------------------------------------------------------------------------------------------


void radioTransmitSetup(){
  byte address[6] = "00001";
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.stopListening();
}
//----------------------------------------------------------------------------------------------




//Sets the center pos for the joysticks based on 20 average positions upon startup
//takes 0.5 seconds to complete
//during which the joysticks should not be touched
void setupJoystick(){
  pinMode(LX, INPUT);
  pinMode(LY, INPUT);
  pinMode(LClick, INPUT_PULLUP);
  pinMode(RX, INPUT);
  pinMode(RY, INPUT);
  pinMode(RClick, INPUT_PULLUP);
  
  short sumLX = 0;
  short sumLY = 0;
  short sumRX = 0;
  short sumRY = 0;
  
  for(int count = 0; count < 20; count++){
    sumLX += analogRead(LX);
    sumLY += analogRead(LY);
    sumRX += analogRead(RX);
    sumRY += analogRead(RY);
  }

  LXCenter = (int)(sumLX / 20);
  LYCenter = (int)(sumLY / 20);
  RXCenter = (int)(sumRX / 20);
  RYCenter = (int)(sumRY / 20);
}
//----------------------------------------------------------------------------------------------




//use if enough dynamic memory left when everything else if done
//map to double value instead of default map function which on returns integer values
//referenced from https://forum.arduino.cc/index.php?topic=361046.0
double mapf(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
