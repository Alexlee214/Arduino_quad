#include <RF24.h>

RF24 radio(A5, A4); // CE, CSN

const int motor1a = 4; 
const int motor1b = 5;//pwm
const int motor2a = 10; //pwm
const int motor2b = 2;
const int motor3a = 8;
const int motor3b = 9; //pwm
const int motor4a = 6; //pwm
const int motor4b = 7; 

int speedMotor = 0;

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





void motorForward(int speedMotor){
  digitalWrite(motor1a, LOW);
  analogWrite(motor1b, speedMotor);

  analogWrite(motor2a, speedMotor);
  digitalWrite(motor2b, LOW);

  digitalWrite(motor3a, LOW);
  analogWrite(motor3b, speedMotor);

  analogWrite(motor4a, speedMotor);
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
