#include <Stepper.h>

#define STEPS 32
#define CLK 2
#define DT 3
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

Stepper stepper(STEPS, IN1, IN3, IN2, IN4);

int previous = 0;
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

void setup() {
  stepper.setSpeed(255);
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
  // stepper.step(STEPS);
  // return;
  // Read the current state of CLK
	currentStateCLK = digitalRead(CLK);

	// If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(DT) != currentStateCLK) {
			stepper.step(STEPS);
      counter--;
			currentDir ="CCW";
		} else {
			// Encoder is rotating CW so increment
			stepper.step(-STEPS);
      counter++;
			currentDir ="CW";
		}

		// Serial.print("Direction: ");
		// Serial.print(currentDir); adminzala113pge
		// Serial.print(" | Counter: ");
		// Serial.println(counter);
	}

	// Remember last CLK state
	lastStateCLK = currentStateCLK;

  // stepper.step(-100);

  delay(1);
}

void updateEncoder(){
	
}
