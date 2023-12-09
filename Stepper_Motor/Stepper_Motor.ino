#include <AccelStepper.h>

#define STEP_PIN 2
#define DIR_PIN 5
#define ENABLE_PIN 13  // Connect to a button to ground to turn on/off
#define RESET_PIN 3
#define SPEED_PIN A7
#define START_STOP_PIN A0

#define D6_PIN 6
#define D7_PIN 7
#define D11_PIN 11
#define D12_PIN 12

AccelStepper stepper(1, STEP_PIN, DIR_PIN);

int MaxSpeed;  
int LowSpeed;

bool hasResetOccurred = false; // Flag to track if the reset operation has occurred

void setup() {
  pinMode(ENABLE_PIN, OUTPUT); // Connect to a button. When low, start or stop the motor
  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(START_STOP_PIN, INPUT_PULLUP);
  pinMode(SPEED_PIN, INPUT);
  
  pinMode(D6_PIN, OUTPUT);
  pinMode(D7_PIN, OUTPUT);
  pinMode(D11_PIN, OUTPUT);
  pinMode(D12_PIN, OUTPUT);

  digitalWrite(ENABLE_PIN, HIGH);  // Enable the motor driver

  stepper.setMaxSpeed(1000);  // Set the initial maximum speed in steps per second
}

void loop() {
  int currentStep = stepper.currentPosition();

  // D6 should be high at 5-50 and 100-200 steps
  if ((currentStep >= 5 && currentStep <= 50) || (currentStep >= 100 && currentStep <= 200)) {
    digitalWrite(D6_PIN, HIGH);
  } else {
    digitalWrite(D6_PIN, LOW);
  }

  // D7 should be high at 15-50 and 120-180 steps
  if ((currentStep >= 15 && currentStep <= 50) || (currentStep >= 120 && currentStep <= 180)) {
    digitalWrite(D7_PIN, HIGH);
  } else {
    digitalWrite(D7_PIN, LOW);
  }

  // D11 should be low at 40-50 and 100-200 steps
  if ((currentStep >= 40 && currentStep <= 50) || (currentStep >= 100 && currentStep <= 200)) {
    digitalWrite(D11_PIN, LOW);
  } else {
    digitalWrite(D11_PIN, HIGH);
  }

  // D12 should be low at 60-70 and 100-150 steps
  if ((currentStep >= 60 && currentStep <= 70) || (currentStep >= 100 && currentStep <= 150)) {
    digitalWrite(D12_PIN, LOW);
  } else {
    digitalWrite(D12_PIN, HIGH);
  }

  if (digitalRead(RESET_PIN) == LOW && !hasResetOccurred) {
    resetMotor();
    hasResetOccurred = true;
  }

  if (digitalRead(START_STOP_PIN) == LOW) {
    toggleMotor();
  }

  if (digitalRead(START_STOP_PIN) == HIGH && stepper.isRunning()) {
    stepper.stop();
  }

  if (digitalRead(SPEED_PIN) == HIGH) {
    stopMotor();
  }

  if (digitalRead(SPEED_PIN) == LOW && digitalRead(START_STOP_PIN) == HIGH) {
    // Read the analog value from A7 and map it to the desired speed range
    int analogValue = analogRead(A7);
    MaxSpeed = map(analogValue, 0, 1023, 0, 700); // Adjust the range as needed
    LowSpeed = MaxSpeed / 2;

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
