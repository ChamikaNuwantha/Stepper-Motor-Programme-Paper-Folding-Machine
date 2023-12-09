#include <AccelStepper.h>

#define STEP_PIN 2
#define DIR_PIN 5
#define START_STOP_PIN 13  // Connect to a button to ground to turn on/off
#define RESET_PIN 3
#define SPEED_PIN A7
#define STOP_PIN A0

AccelStepper stepper(1, STEP_PIN, DIR_PIN);

int MaxSpeed;
int LowSpeed = MaxSpeed/2;

bool hasResetOccurred = false; // Flag to track if the reset operation has occurred
bool hasStartOccurred = false;


void setup() {
  pinMode(STOP_PIN, INPUT_PULLUP); // Connect to a button. When low, stop the motor
  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(START_STOP_PIN, INPUT_PULLUP);
  pinMode(SPEED_PIN, INPUT);

  digitalWrite(ENABLE_PIN, HIGH);  // Enable the motor driver

  stepper.setMaxSpeed(1000);  // Set the initial maximum speed in steps per second
}

void loop() {
  // if (digitalRead(STOP_PIN) == LOW && !hasResetOccurred) {
  //     resetMotor();
  //     hasResetOccurred = true;
  // }

  if (digitalRead(RESET_PIN) == LOW && !hasResetOccurred) {
    resetMotor();
    hasResetOccurred = true;
  }

  if (digitalRead(START_STOP_PIN) == LOW && !hasStartOccurred) {
    toggleMotor();
    hasStartOccurred = true;
  }

  if (digitalRead(START_STOP_PIN) == LOW && stepper.isRunning()) {
    stepper.stop();
    hasStartOccurred = false;
  }

  if (digitalRead(SPEED_PIN) == HIGH) {
    stopMotor();
  }

  if (digitalRead(SPEED_PIN) == LOW && digitalRead(START_STOP_PIN) == HIGH) {
    int analogValue = analogRead(A7);
    MaxSpeed = map(analogValue, 0, 1023, 0, 700); // Adjust the range as needed.. initially set the maximum pulse per second as 700

    stepper.setMaxSpeed(MaxSpeed);
    runMotor();
  }
}

void resetMotor() {
  stepper.setCurrentPosition(0);
}

void toggleMotor() {
  if (stepper.isRunning()) {
    stepper.stop();
  } else {
    runMotor();
  }
}

void stopMotor() {
  stepper.stop();
}

void runMotor() {
  for (int i = 0; i < 50; ++i) {
    int currentSpeed = map(i, 0, 49, 0, MaxSpeed);  // Map the speed from 0 to maxspeed in 50 steps
    stepper.setSpeed(currentSpeed);
    stepper.runSpeedToPosition();
  }

  stepper.moveTo(50);  // Move 50 steps maintaining constant speed
  stepper.runToPosition();

  // delay(1000);

  for (int i = 0; i < 100; ++i) {
    stepper.setSpeed(MaxSpeed);  // Set speed for the first 100 steps
    stepper.runSpeedToPosition();
  }

  for (int i = 0; i < 50; ++i) {
    stepper.setSpeed(LowSpeed);  // Slow down in 50 steps
    stepper.runSpeedToPosition();
  }

  for (int i = 0; i < 50; ++i) {
    stepper.setSpeed(MaxSpeed);  // Speed up back to normal speed in 50 steps
    stepper.runSpeedToPosition();
  }

  for (int i = 0; i < 100; ++i) {
    stepper.setSpeed(MaxSpeed);  // Continue at normal speed for the next 100 steps
    stepper.runSpeedToPosition();
  }
}
