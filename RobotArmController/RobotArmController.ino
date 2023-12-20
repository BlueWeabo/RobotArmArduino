#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#define CE 7
#define CSN 8
#define MOTOR_BASE_CLK 2
#define MOTOR_BASE_DT 3
#define MOTOR_SWIVEL_CLK 4
#define MOTOR_SWIVEL_DT 5
#define MOTOR_JOINT_ONE_CLK 6
#define MOTOR_JOINT_ONE_DT 7
#define MOTOR_JOINT_TWO_CLK 9
#define MOTOR_JOINT_TWO_DT 10
#define MOTOR_HAND_CLK 11
#define MOTOR_HAND_DT 12
#define MOTOR_SPEED A0
#define GRABBER 13

RF24 radio(CE, CSN);

int previous = 0;
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";

char address[] = "00001";

void setup() {
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);

  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);

  Serial.begin(9600);

  if (!radio.begin()) {
    Serial.println(F("radio hardware not responding!"));
    while (1) {} // hold program in infinite loop to prevent subsequent errors
  }

  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  currentStateCLK = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      counter--;
      currentDir ="CCW";
    } else {
      // Encoder is rotating CW so increment
      counter++;
      currentDir ="CW";
    }

    // Serial.print("Direction: ");
    // Serial.print(currentDir);
    // Serial.print(" | Counter: ");
    // Serial.println(counter);
  }

  // Remember last CLK state
  lastStateCLK = currentStateCLK;
  delay(1);
}
