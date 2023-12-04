// pins for the ir sensors
const int pinIRLeft = A0;
const int pinIRMiddle = A1;
const int pinIRight = A2;

// pins for the motors
const int pinMotorLA = 13;
const int pinMotorLB = 12;
const int pinMotorRA = 8;
const int pinMotorRB = 7;
const int pinMotorLEnable = 6;
const int pinMotorREnable = 5;

// speed of the motors
int motorLSpeed = 0;
int motorRSpeed = 0;
int motorSpeed;

// parameters used for the PID controller
float kp = 1;
float ki = 0;
float kd = 0;

// values passed to the PID controller
int p = 1;
int i = 0;
int d = 0;

// values used for computing the derivative
int error = 0;
int lastError = 0;

// constants for the motors speed
const int minSpeed = -255;
const int maxSpeed = 255;
const int baseSpeed = 255;

const int sensorCount = 3;
int sensorValues[sensorCount];

void setup() {
  pinMode(pinIRLeft, INPUT);
  pinMode(pinIRMiddle, INPUT);
  pinMode(pinIRight, INPUT);

  pinMode(pinMotorLA, OUTPUT);
  pinMode(pinMotorLB, OUTPUT);
  pinMode(pinMotorRA, OUTPUT);
  pinMode(pinMotorRB, OUTPUT);

  pinMode(pinMotorLEnable, OUTPUT);
  pinMode(pinMotorREnable, OUTPUT);

  lastError = computeError();
}

void loop() {
  pidControl(kp, ki, kd);
  computeMotorsSpeed();
  setMotorSpeed(motorLSpeed, motorRSpeed);
}

int computeError() {
  int sensorAverage = 0, sensorSum = 0;

  for (int i = 0; i < 3; i++) {
    sensorValues[i] = analogRead(i);
    sensorAverage += sensorValues[i] * i * 1000;
    sensorSum += sensorValues[i];
  }

  return map(sensorAverage / sensorSum, 0, 2000, -20, 20);
}

void pidControl(float kp, float ki, float kd) {
  error = computeError();  // need to find how to get error

  p = error;
  i += error;
  d = error - lastError;
  lastError = error;

  motorSpeed = kp * p + ki * i + kd * d;
}

void computeMotorsSpeed() {
  motorLSpeed = baseSpeed;
  motorRSpeed = baseSpeed;

  if (error < 0) {
    motorLSpeed += motorSpeed;
  } else if (error > 0) {
    motorRSpeed -= motorSpeed;
  }

  motorLSpeed = constrain(motorLSpeed, minSpeed, maxSpeed);
  motorRSpeed = constrain(motorRSpeed, minSpeed, maxSpeed);
}

void setMotorSpeed(int motorLSpeed, int motorRSpeed) {
  motorLSpeed = -motorLSpeed;
  if (motorLSpeed == 0) {
    digitalWrite(pinMotorLA, LOW);
    digitalWrite(pinMotorLB, LOW);
    analogWrite(pinMotorLEnable, motorLSpeed);
  } else {
    if (motorLSpeed > 0) {
      digitalWrite(pinMotorLA, HIGH);
      digitalWrite(pinMotorLB, LOW);
      analogWrite(pinMotorLEnable, motorLSpeed);
    }
    if (motorLSpeed < 0) {
      digitalWrite(pinMotorLA, LOW);
      digitalWrite(pinMotorLB, HIGH);
      analogWrite(pinMotorLEnable, -motorLSpeed);
    }
  }
  if (motorRSpeed == 0) {
    digitalWrite(pinMotorRA, LOW);
    digitalWrite(pinMotorRB, LOW);
    analogWrite(pinMotorREnable, motorRSpeed);
  } else {
    if (motorRSpeed > 0) {
      digitalWrite(pinMotorRA, HIGH);
      digitalWrite(pinMotorRB, LOW);
      analogWrite(pinMotorREnable, motorRSpeed);
    }
    if (motorRSpeed < 0) {
      digitalWrite(pinMotorRA, LOW);
      digitalWrite(pinMotorRB, HIGH);
      analogWrite(pinMotorREnable, -motorRSpeed);
    }
  }
}
