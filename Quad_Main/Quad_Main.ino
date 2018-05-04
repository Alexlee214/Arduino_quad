//modification: add mpu6050, and set roll pitch yaw based on the values read


#include <RF24.h>

RF24 radio(A5, A4); // CE, CSN

const byte motor1a = 4; 
const byte motor1b = 5;//pwm
const byte motor2a = 10; //pwm
const byte motor2b = 2;
const byte motor3a = 8;
const byte motor3b = 9; //pwm
const byte motor4a = 6; //pwm
const byte motor4b = 7; 

byte speedMotor1 = 0;  //clockwise
byte speedMotor2 = 0;  //counter-clockwise
byte speedMotor3 = 0;  //clockwise
byte speedMotor4 = 0;  //counter-clockwise

short yawOffset = 0;
short pitchOffset = 0;
short rollOffset = 0;


byte normalizeSpeed(short rawSpeed);
void assignSpeed(short rawspeed1, short rawspeed2, short rawspeed3, short rawspeed4);
byte maxSpeed(byte speed1, byte speed2, byte speed3, byte speed4);
void takeoffLand(short data);
void setRoll(short data);
void setPitch(short data);
void setYaw(short data);
void setElevation(short data);
void radioReceiveSetup();
void motorForward(byte speedMotor1, byte speedMotor2, byte speedMotor3, byte speedMotor4);
void initializeMotors();


void setup() {
  // put your setup code here, to run once:
  initializeMotors();
  radioReceiveSetup();
  Serial.begin(9600);
}

void loop() {
    if (radio.available()) {
    short data[6];
    radio.read(&data, sizeof(data));
    /*
    Serial.print("Elevation: ");
    Serial.print(data[0]);
    Serial.print("  ");
    Serial.print("Yaw: ");
    Serial.print(data[1]);
    Serial.print("  ");
    Serial.print("Pitch: ");
    Serial.print(data[2]);
    Serial.print("  ");
    Serial.print("Roll: ");
    Serial.print(data[3]);
    Serial.print("  ");
    Serial.print("Left click: ");
    Serial.print(data[4]);
    Serial.print("  ");
    Serial.print("Right click: ");
    Serial.print(data[5]);
    Serial.println("  ");
    */
    
    setElevation(data[0]);
    setYaw(data[1]);
    setPitch(data[2]);
    setRoll(data[3]);
    takeoffLand(data[4]);
    
    Serial.print("motor1: ");
    Serial.print(speedMotor1);
    Serial.print("  ");
    Serial.print("motor2: ");
    Serial.print(speedMotor2);
    Serial.print("  ");
    Serial.print("motor3: ");
    Serial.print(speedMotor3);
    Serial.print("  ");
    Serial.print("motor4: ");
    Serial.print(speedMotor4);
    Serial.println("  ");
  }
}

void initializeMotors(){
  pinMode(motor1a, INPUT);
  pinMode(motor1b, INPUT);
  pinMode(motor2a, INPUT);
  pinMode(motor2b, INPUT);
  pinMode(motor3a, INPUT);
  pinMode(motor3b, INPUT);
  pinMode(motor4a, INPUT);
  pinMode(motor4b, INPUT);
  
  analogWrite(motor1b, 0);
  digitalWrite(motor1a, LOW);
  digitalWrite(motor2b, LOW);
  analogWrite(motor2a, 0);
  analogWrite(motor3b, 0);
  digitalWrite(motor3a, LOW);
  digitalWrite(motor4b, LOW);
  analogWrite(motor4a, 0);
}



//given the speed that each motor should be set to, set each motor speed
void motorForward(byte speedMotor1, byte speedMotor2, byte speedMotor3, byte speedMotor4){
  digitalWrite(motor1a, LOW);
  analogWrite(motor1b, speedMotor1);

  analogWrite(motor2a, speedMotor2);
  digitalWrite(motor2b, LOW);

  digitalWrite(motor3a, LOW);
  analogWrite(motor3b, speedMotor3);

  analogWrite(motor4a, speedMotor4);
  digitalWrite(motor4b, LOW);
}  




void radioReceiveSetup(){
  byte address[6] = "00001";
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.startListening();
}




void setElevation(short data){
  if(data != 0){
    short speed1, speed2, speed3, speed4;
    speed1 = speedMotor1 + data;
    speed2 = speedMotor2 + data;
    speed3 = speedMotor3 + data;
    speed4 = speedMotor4 + data;
  
    assignSpeed(speed1, speed2, speed3, speed4);
  }  
}


void setYaw(short data){
  short speed1, speed2, speed3, speed4;
  //if no offset, and yawOffset set yaw to 0
  if(data != 0 && yawOffset == 0){
    yawOffset = data;
    speed1 = speedMotor1 + data;
    speed2 = speedMotor2 - data;
    speed3 = speedMotor3 + data;
    speed4 = speedMotor4 - data;

    assignSpeed(speed1, speed2, speed3, speed4);
    
  }else{
    //if joystick returned to center position
    if(data == 0 && yawOffset != 0){
      speed1 = speedMotor1 - yawOffset;
      speed2 = speedMotor2 + yawOffset;
      speed3 = speedMotor3 - yawOffset;
      speed4 = speedMotor4 + yawOffset;
      yawOffset = 0;
    }  
  }  
}


void setPitch(short data){
  short speed1, speed2, speed3, speed4;
  //if no offset, and pitchOffset set pitch to 0
  if(data != 0 && pitchOffset == 0){
    pitchOffset = data;
    speed1 = speedMotor1 - data;
    speed2 = speedMotor2 - data;
    speed3 = speedMotor3 + data;
    speed4 = speedMotor4 + data;

    assignSpeed(speed1, speed2, speed3, speed4);

  //if joystick returned to center position  
  }else if(data == 0 && pitchOffset != 0){
    speed1 = speedMotor1 + pitchOffset;
    speed2 = speedMotor2 + pitchOffset;
    speed3 = speedMotor3 - pitchOffset;
    speed4 = speedMotor4 - pitchOffset;
    pitchOffset = 0;
  }
}


void setRoll(short data){
  short speed1, speed2, speed3, speed4;
  //if no offset, and rollOffset set roll to 0
  if(data != 0 && rollOffset == 0){
    rollOffset = data;
    speed1 = speedMotor1 + data;
    speed2 = speedMotor2 - data;
    speed3 = speedMotor3 - data;
    speed4 = speedMotor4 + data;

    assignSpeed(speed1, speed2, speed3, speed4);

  //if joystick returned to center position  
  }else if(data == 0 && & rollOffset != 0){
    speed1 = speedMotor1 - rollOffset;
    speed2 = speedMotor2 + rollOffset;
    speed3 = speedMotor3 + rollOffset;
    speed4 = speedMotor4 - rollOffset;
    rollOffset = 0;
  }
}



//modify here to adjust speed of auto takeoff, landing
void takeoffLand(short data){
  //if the quad is at rest, take off
  //add code for stabilizing the quad with mpu6050
  if (speedMotor1 == 0 && speedMotor2 == 0 && speedMotor3 == 0 && speedMotor4 == 0 && data == 1){
    byte countSpeed = 0;
    for(countSpeed = 0; countSpeed < 80; countSpeed++){
      motorForward(countSpeed, countSpeed, countSpeed, countSpeed);
      delay(50);
      Serial.print("motor1: ");
      Serial.print(countSpeed);
      Serial.print("  ");
      Serial.print("motor2: ");
      Serial.print(countSpeed);
      Serial.print("  ");
      Serial.print("motor3: ");
      Serial.print(countSpeed);
      Serial.print("  ");
      Serial.print("motor4: ");
      Serial.print(countSpeed);
      Serial.println("  ");
    }
    //update the speed after it has taken off
    speedMotor1 = countSpeed;
    speedMotor2 = countSpeed;
    speedMotor3 = countSpeed;
    speedMotor4 = countSpeed;

  //if quad is not at rest, land it  
  }else if(speedMotor1 != 0 && speedMotor2 != 0 && speedMotor3 != 0 && speedMotor4 != 0 && data == 1){
    /*
    while(maxSpeed(speedMotor1, speedMotor2, speedMotor3, speedMotor4) >= 1){
      if (speedMotor1 >= 1) speedMotor1--;
      else speedMotor1 = 0;
      if (speedMotor2 >= 1) speedMotor2--;
      else speedMotor2 = 0;
      if (speedMotor3 >= 1) speedMotor3--;
      else speedMotor3 = 0;
      if (speedMotor4 >= 1) speedMotor4--;
      else speedMotor4 = 0;
      motorForward(speedMotor1, speedMotor2, speedMotor3, speedMotor4);
      delay(50);
    }
    */
    short speed1 = speedMotor1;
    short speed2 = speedMotor2;
    short speed3 = speedMotor3;
    short speed4 = speedMotor4;

    //if any of the values are not 0, keep decrementing
    while( ((speed1--) | (speed2--) | (speed3--) | (speed4--) ) != 0){
      assignSpeed(speed1, speed2, speed3, speed4);
      motorForward(speedMotor1, speedMotor2, speedMotor3, speedMotor4);
      Serial.print("motor1: ");
      Serial.print(speedMotor1);
      Serial.print("  ");
      Serial.print("motor2: ");
      Serial.print(speedMotor2);
      Serial.print("  ");
      Serial.print("motor3: ");
      Serial.print(speedMotor3);
      Serial.print("  ");
      Serial.print("motor4: ");
      Serial.print(speedMotor4);
      Serial.println("  ");
      delay(50);
    }
  }
}


//returns maximum speed among all motors
byte maxSpeed(byte speed1, byte speed2, byte speed3, byte speed4){
  return max(max(speed1, speed2), max(speed3, speed4));
}



//sets the speed that the motor should be set to
void assignSpeed(short rawSpeed1, short rawSpeed2, short rawSpeed3, short rawSpeed4){
    speedMotor1 = normalizeSpeed(rawSpeed1);
    speedMotor2 = normalizeSpeed(rawSpeed2);
    speedMotor3 = normalizeSpeed(rawSpeed3);
    speedMotor4 = normalizeSpeed(rawSpeed4);
}  


//Adjust raw speed so it does not go beyond 255 or below 0
byte normalizeSpeed(short rawSpeed){
  if(rawSpeed >= 255) return 255;
  else if (rawSpeed <= 0) return 0;
  else return rawSpeed;
}  

