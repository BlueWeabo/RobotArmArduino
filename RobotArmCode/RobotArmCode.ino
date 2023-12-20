#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#include <Stepper.h>
#include <Servo.h>

#define MOTOR_STEPS 32
const int MOTOR_BASE[] = {1,2,3,4};
const int MOTOR_SWIVEL[] = {1,2,3,4};
const int MOTOR_JOINT_ONE[] = {1,2,3,4};
const int MOTOR_JOINT_TWO[] = {1,2,3,4};
const int MOTOR_HAND[] = {1,2,3,4};
#define SERVO_GRABBER 1
#define CLK 2
#define DT 3
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

Stepper baseMotor(MOTOR_STEPS, MOTOR_BASE[0], MOTOR_BASE[2], MOTOR_BASE[1], MOTOR_BASE[3]);
Stepper swivelMotor(MOTOR_STEPS, MOTOR_SWIVEL[0], MOTOR_SWIVEL[2], MOTOR_SWIVEL[1], MOTOR_SWIVEL[3]);
Stepper jointOneMotor(MOTOR_STEPS, MOTOR_JOINT_ONE[0], MOTOR_JOINT_ONE[2], MOTOR_JOINT_ONE[1], MOTOR_JOINT_ONE[3]);
Stepper jointTwoMotor(MOTOR_STEPS, MOTOR_JOINT_TWO[0], MOTOR_JOINT_TWO[2], MOTOR_JOINT_TWO[1], MOTOR_JOINT_TWO[3]);
Stepper handMotor(MOTOR_STEPS, MOTOR_HAND[0], MOTOR_HAND[2], MOTOR_HAND[1], MOTOR_HAND[3]);
Servo grabberServo(SERVO_GRABBER);

int previous = 0;
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

void setup() {
  baseMotor.setSpeed(255);
	// Set encoder pins as inputs
	pinMode(CLK,INPUT);
	pinMode(DT,INPUT);

	// Setup Serial Monitor
	//Serial.begin(9600);

	// Read the initial state of CLK
	lastStateCLK = digitalRead(CLK);

  // attachInterrupt(0, updateEncoder, CHANGE);
	// attachInterrupt(1, updateEncoder, CHANGE);
}

void loop() {
  // baseMotor.step(STEPS);
  // return;
  // Read the current state of CLK
	currentStateCLK = digitalRead(CLK);

	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(DT) != currentStateCLK) {
			baseMotor.step(MOTOR_STEPS);
      counter--;
			currentDir ="CCW";
		} else {
			// Encoder is rotating CW so increment
			baseMotor.step(-MOTOR_STEPS);
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

  // baseMotor.step(-100);

  delay(1);
}

void updateEncoder(){
	
}
