#include <RF24.h>

RF24 radio(A5, A4); // CE, CSN

const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radioReceiveSetup();
}

void loop() {
  if (radio.available()) {
    short data[6];
    radio.read(&data, sizeof(data));
    Serial.print(data[0]);
    Serial.print("  ");
    Serial.print(data[1]);
    Serial.print("  ");
    Serial.print(data[2]);
    Serial.print("  ");
    Serial.print(data[3]);
    Serial.print("  ");
    Serial.print(data[4]);
    Serial.print("  ");
    Serial.print(data[5]);
    Serial.println("  ");
  }
}


void radioReceiveSetup(){
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.startListening();
}

