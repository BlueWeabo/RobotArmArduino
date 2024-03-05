#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

// Set pins
#define CE 7
#define CSN 8
#define MOTOR_BASE_SWIVEL_CLK 2
#define MOTOR_BASE_SWIVEL_DT 3
#define MOTOR_BASE_SWIVEL_SW 4
#define MOTOR_JOINTS_CLK 5
#define MOTOR_JOINTS_DT 6
#define MOTOR_JOINTS_SW 10
#define MOTOR_HAND_CLK 0
#define MOTOR_HAND_DT 1
#define MOTOR_HAND_SW 9
#define MOTOR_SPEED A0

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
const byte ADDRESS[6] = "00001";

// Set up NRF24 Module object
RF24 radio(CE, CSN);

int baseOrSwivel = 0; // Move base by default
int jointOneOrTwo = 0; // Move joint one by default
int openedOrClosed = 0; // Hand Opened by default

// Variables storing last state and current state of CLK
int lastStateJointsCLK = 0;
int lastStateBaseSwivelCLK = 0;
int lastStateHandCLK = 0;

// Store last speed we set
int currentSpeed = 0;

// Function to check the pins and give back the direction
// Returns: -1 when rotated CCW, 0 when it wasn't rotated, 1 when rotated CW
int determineRotationDirectionBaseSwivel() {
  int currentState = digitalRead(MOTOR_BASE_SWIVEL_CLK);
  int toReturn = 0;
  if (currentState != lastStateBaseSwivelCLK  && currentState){
    // Check whether we are rotating clockwise (CW) or counter-clockwise (CCW)
    if (digitalRead(MOTOR_BASE_SWIVEL_DT) != currentState) {
      // if DT pin activated first then we are rotating CCW
      //Serial.println("CCW-B");
      toReturn = -1;
    } else {
      // Otherwise it means CLK pin activated first and we are rotating CW
      //Serial.println("CW-B");
      toReturn = 1;
    }
  }
  lastStateBaseSwivelCLK = currentState;
  return toReturn;
}


// Function to check the pins and give back the direction
// Returns: -1 when rotated CCW, 0 when it wasn't rotated, 1 when rotated CW
int determineRotationDirectionJoints() {
  int currentState = digitalRead(MOTOR_JOINTS_CLK);
  int toReturn = 0;
  if (currentState != lastStateJointsCLK  && currentState){
    // Check whether we are rotating clockwise (CW) or counter-clockwise (CCW)
    if (digitalRead(MOTOR_JOINTS_DT) != currentState) {
      // if DT pin activated first then we are rotating CCW
      //Serial.println("CCW-J");
      toReturn = -1;
    } else {
      // Otherwise it means CLK pin activated first and we are rotating CW
      //Serial.println("CW-J");
      toReturn = 1;
    }
  }
  lastStateJointsCLK = currentState;
  return toReturn;
}

// Function to check the pins and give back the direction
// Returns: -1 when rotated CCW, 0 when it wasn't rotated, 1 when rotated CW
int determineRotationDirectionHand() {
  int currentState = digitalRead(MOTOR_HAND_CLK);
  int toReturn = 0;
  if (currentState != lastStateHandCLK  && currentState){
    // Check whether we are rotating clockwise (CW) or counter-clockwise (CCW)
    if (digitalRead(MOTOR_HAND_DT) != currentState) {
      // if DT pin activated first then we are rotating CCW
      //Serial.println("CCW-H");
      toReturn = -1;
    } else {
      // Otherwise it means CLK pin activated first and we are rotating CW
      //Serial.println("CW-H");
      toReturn = 1;
    }
  }
  lastStateHandCLK = currentState;
  return toReturn;
}

void setup() {
  // Set all the needed pins to INPUT
  pinMode(MOTOR_BASE_SWIVEL_CLK, INPUT);
  pinMode(MOTOR_BASE_SWIVEL_DT, INPUT);
  pinMode(MOTOR_BASE_SWIVEL_SW, INPUT_PULLUP);
  pinMode(MOTOR_JOINTS_CLK, INPUT);
  pinMode(MOTOR_JOINTS_DT, INPUT);
  pinMode(MOTOR_JOINTS_SW, INPUT_PULLUP);
  pinMode(MOTOR_HAND_CLK, INPUT);
  pinMode(MOTOR_HAND_DT, INPUT);
  pinMode(MOTOR_HAND_SW, INPUT_PULLUP);

  // Read the initial state of CLK
  lastStateBaseSwivelCLK = digitalRead(MOTOR_BASE_SWIVEL_CLK);
  lastStateJointsCLK = digitalRead(MOTOR_BASE_SWIVEL_CLK);
  lastStateHandCLK = digitalRead(MOTOR_BASE_SWIVEL_CLK);

  // Read initial speed value
  currentSpeed = map(0, 1023, 0, 255, analogRead(MOTOR_SPEED));

  // Enable Serial communivation for debugging
  //Serial.begin(9600);
  Serial.end();

  // Check if radio has been connected correctly
  if (!radio.begin()) {
    Serial.println(F("radio hardware not responding!"));
    while (1) {
      Serial.println("Radio bugged");
    } // hold program in infinite loop to prevent subsequent errors
  }

  // Open the address of where we will be sending information to
  radio.openWritingPipe(ADDRESS);
  // Disable listening to make sure we arne't getting any signals from elsewhere to our transmitter
  radio.stopListening();
  // Send the speed we have currently 
  byte buf[3] = {highByte(CHANGE_SPEED), lowByte(CHANGE_SPEED), lowByte(currentSpeed)};
  radio.write(&buf, sizeof(buf));
}

void loop() {
  int baseSwivelRotation = determineRotationDirectionBaseSwivel();
  int jointsRotation = determineRotationDirectionJoints();
  int handRotation = determineRotationDirectionHand();
  int basesw = digitalRead(MOTOR_BASE_SWIVEL_SW);
  int jointsw = digitalRead(MOTOR_JOINTS_SW);
  int handsw = digitalRead(MOTOR_HAND_SW);
  if (!basesw) {
    baseOrSwivel = !baseOrSwivel;
    //Serial.println("Switched base");
    delay(200);
  }
  if (!jointsw) {
    jointOneOrTwo = !jointOneOrTwo;
    //Serial.println("Switched joints");
    delay(200);
  }
  char buffer[3] = "";
  if (!handsw) {
    openedOrClosed = !openedOrClosed;
    //Serial.println("Switched hand");
    if (openedOrClosed) {
      buffer[0] = highByte(OPEN_HAND);
      buffer[1] = lowByte(OPEN_HAND);
    } else {
      buffer[0] = highByte(CLOSE_HAND);
      buffer[1] = lowByte(CLOSE_HAND);
    }
    radio.write(&buffer, sizeof(buffer));
    delay(200);
  }
    if (baseOrSwivel) {
      if (baseSwivelRotation == -1) {
        buffer[0] = highByte(MOVE_SWIVEL_CCW);
        buffer[1] = lowByte(MOVE_SWIVEL_CCW);
        //Serial.println("Moving  Swivel CCW");
        radio.write(&buffer, sizeof(buffer));
      } else if (baseSwivelRotation == 1) {
        buffer[0] = highByte(MOVE_SWIVEL_CW);
        buffer[1] = lowByte(MOVE_SWIVEL_CW);
        //Serial.println("Moving Swivel CW");
        radio.write(&buffer, sizeof(buffer));
      }
    } else {
      if (baseSwivelRotation == -1) {
        buffer[0] = highByte(MOVE_BASE_CCW);
        buffer[1] = lowByte(MOVE_BASE_CCW);
        //Serial.println("Moving Base CCW");
        radio.write(&buffer, sizeof(buffer));
      } else if (baseSwivelRotation == 1) {
        buffer[0] = highByte(MOVE_BASE_CW);
        buffer[1] = lowByte(MOVE_BASE_CW);
        //Serial.println("Moving Base CW");
        radio.write(&buffer, sizeof(buffer));
      }
    }

    if (jointOneOrTwo) {
      if (jointsRotation == -1) {
        buffer[0] = highByte(MOVE_JOINT_TWO_CCW);
        buffer[1] = lowByte(MOVE_JOINT_TWO_CCW);
        radio.write(&buffer, sizeof(buffer));
      } else {
        buffer[0] = highByte(MOVE_JOINT_TWO_CW);
        buffer[1] = lowByte(MOVE_JOINT_TWO_CW);
        radio.write(&buffer, sizeof(buffer));
      }
    } else {
      if (jointsRotation == -1) {
        buffer[0] = highByte(MOVE_JOINT_ONE_CCW);
        buffer[1] = lowByte(MOVE_JOINT_ONE_CCW);
        radio.write(&buffer, sizeof(buffer));
      } else if (jointsRotation == 1) {
        buffer[0] = highByte(MOVE_JOINT_ONE_CW);
        buffer[1] = lowByte(MOVE_JOINT_ONE_CW);
        radio.write(&buffer, sizeof(buffer));
      }
    }
    

    if (handRotation == -1) {
      buffer[0] = highByte(MOVE_HAND_CCW);
      buffer[1] = lowByte(MOVE_HAND_CCW);
      radio.write(&buffer, sizeof(buffer));
    } else if (handRotation == 1) {
      buffer[0] = highByte(MOVE_HAND_CW);
      buffer[1] = lowByte(MOVE_HAND_CW);
      radio.write(&buffer, sizeof(buffer));
    }

  int newS = map(0, 1013, 0, 255, analogRead(MOTOR_SPEED));
  if (currentSpeed != newS) {
    buffer[0] = highByte(MOVE_SWIVEL_CCW);
    buffer[1] = lowByte(MOVE_SWIVEL_CCW);
    buffer[2] = newS;
    radio.write(&buffer, sizeof(buffer));
  }
  delay(10);
}

