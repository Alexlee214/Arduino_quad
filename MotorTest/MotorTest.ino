const int motor1 = 5;
const int motor2 = 6;

void setup() {
  // put your setup code here, to run once:
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  analogWrite(motor1, 0);
  analogWrite(motor2, 0);
}

void loop() {
  delay(1000);
  forward(50, 1000);
  stopMotor();
  delay(1000);
  backward(50, 1000);
  stopMotor();

  for(int con = 0; con <= 255; con += 10){
    forward(con, 200);
  }
  stopMotor();
}

void forward(int speedP, int timeS){
  analogWrite(motor1, speedP);
  analogWrite(motor2, 0);
  delay(timeS);
}

void backward(int speedP, int timeS){
  analogWrite(motor2, speedP);
  analogWrite(motor1, 0);
  delay(timeS);
}

void stopMotor(){
  analogWrite(motor2, 0);
  analogWrite(motor1, 0);
}
