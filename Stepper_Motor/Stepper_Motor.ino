#include <Button.h>

#define STEP_PIN 4
int fullPulse = 700;
int halfPulse = 350;
int rpm = 200;


int pulse;
int rpm_t;
Button sw12(12);
Button sw13(9);
Button swA3(A3);

void setup() {
  Serial.begin(9600);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(A7, INPUT_PULLUP);

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);

  pinMode(A3, OUTPUT);
  pinMode(11, OUTPUT);

  pinMode(3, INPUT_PULLUP);

  sw12.begin();
  sw13.begin();
  swA3.begin();
}

int stepCount = 0;
bool motorEnable = true;
bool timeTrigger = false;
long time = 0;
void loop() {
  int potRead = analogRead(A7);

  if (digitalRead(3) == LOW) {
    Serial.println("Reset...");
    stepCount = 0;
    motorEnable = true;
    timeTrigger = false;
    time = 0;
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
  }

  if (sw13.toggled()) {
    if (sw13.read() == Button::PRESSED) {
      timeTrigger = !timeTrigger;
      time = millis();
    }
  }

  if (timeTrigger) {
    int currTime = millis() - time;
    if (currTime > 500 && currTime < 1200) {
      digitalWrite(A4, HIGH);
    } else {
      digitalWrite(A4, LOW);
    }
    if (currTime > 1000 && currTime < 1400) {
      digitalWrite(A5, HIGH);
    } else {
      digitalWrite(A5, LOW);
    }
  }

  switch (stepCount) {
    case 15:
      digitalWrite(6, HIGH);
      break;
    case 20:
      pulse = halfPulse;
      break;
    case 40:
      digitalWrite(7, HIGH);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      break;
    case 50:
      digitalWrite(6, LOW);
      break;
    case 60:
      digitalWrite(7, LOW);
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      break;
    case 65:
      digitalWrite(6, HIGH);
      break;
    case 80:
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      break;
    case 100:
      pulse = map(potRead, 0, 1023, 0, fullPulse);
      break;
    case 330:
      pulse = halfPulse;
      break;
    case 430:
      pulse = map(potRead, 0, 1023, 0, fullPulse);
      break;
  }

  rpm_t = 80000000 / 2 / pulse / rpm;

  if (sw12.toggled()) {
    if (sw12.read() == Button::PRESSED) {
      Serial.println("12 Triggered");
      motorEnable = !motorEnable;
    }
  }

  if (sw13.toggled()) {
    if (sw13.read() == Button::PRESSED) {
      Serial.println("13 Triggered");
      motorEnable = !motorEnable;
    }
  }

  if (motorEnable) {
    stepCount++;
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(rpm_t);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(rpm_t);
  }
}