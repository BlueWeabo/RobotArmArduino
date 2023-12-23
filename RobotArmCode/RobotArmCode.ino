#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#include <Stepper.h>
#include <Servo.h>

// Define amount of steps a motor can do
#define MOTOR_STEPS 32

// Set pins
const int MOTOR_BASE[] = {22,23,24,25};
const int MOTOR_SWIVEL[] = {26,27,28,29};
const int MOTOR_JOINT_ONE[] = {30,31,32,33};
const int MOTOR_JOINT_TWO[] = {34,35,36,36};
const int MOTOR_HAND[] = {37,38,39,40};
#define SERVO_GRABBER 2
#define CE 7
#define CSN 8

// Define commands
// Define commands
enum Commands {
  MOVE_BASE_CW = 1 << 0,
  MOVE_BASE_CCW = 1 << 1,
  MOVE_SWIVEL_CW = 1 << 2,
  MOVE_SWIVEL_CCW = 1 << 3,
  MOVE_JOINT_ONE_CW = 1 << 4,
  MOVE_JOINT_ONE_CCW = 1 << 5,
  MOVE_JOINT_TWO_CW = 1 << 6,
  MOVE_JOINT_TWO_CCW = 1 << 7,
  MOVE_HAND_CW = 1 << 8,
  MOVE_HAND_CCW = 1 << 9,
  OPEN_HAND = 1 << 10,
  CLOSE_HAND = 1 << 11,
  CHANGE_SPEED = 1 << 12
};

// Set address for NRF24 module
const byte ADDRESS[5] = "00001";

// Set up NRF24 Module object
RF24 radio(CE, CSN);

// Define Motor objects
Stepper baseMotor(MOTOR_STEPS, MOTOR_BASE[0], MOTOR_BASE[2], MOTOR_BASE[1], MOTOR_BASE[3]);
Stepper swivelMotor(MOTOR_STEPS, MOTOR_SWIVEL[0], MOTOR_SWIVEL[2], MOTOR_SWIVEL[1], MOTOR_SWIVEL[3]);
Stepper jointOneMotor(MOTOR_STEPS, MOTOR_JOINT_ONE[0], MOTOR_JOINT_ONE[2], MOTOR_JOINT_ONE[1], MOTOR_JOINT_ONE[3]);
Stepper jointTwoMotor(MOTOR_STEPS, MOTOR_JOINT_TWO[0], MOTOR_JOINT_TWO[2], MOTOR_JOINT_TWO[1], MOTOR_JOINT_TWO[3]);
Stepper handMotor(MOTOR_STEPS, MOTOR_HAND[0], MOTOR_HAND[2], MOTOR_HAND[1], MOTOR_HAND[3]);
Servo grabber;

void setup() {
  Serial.begin(9600);
  baseMotor.setSpeed(255);
  swivelMotor.setSpeed(25);
  jointOneMotor.setSpeed(255);
  jointTwoMotor.setSpeed(255);
  handMotor.setSpeed(255);
  grabber.attach(SERVO_GRABBER);

  if (!radio.begin()) {
    Serial.println(F("radio hardware not responding!"));
    while (1) {
      Serial.println("Radio bugged");
    } // hold program in infinite loop to prevent subsequent errors
  }
  radio.openReadingPipe(0, ADDRESS);
  radio.startListening();
}

void loop() {
  if (!radio.available()) {
    return;
  }
	char buffer[32];
  radio.read(&buffer, sizeof(buffer));
  int command = buffer[0] * 256 + buffer[1];
  switch (command) {
    case MOVE_BASE_CW: {
      Serial.println("Moving Base CW");
      break;
    }
    case MOVE_BASE_CCW: {
      Serial.println("Moving Base CW");
      break;
    }
  }
}
