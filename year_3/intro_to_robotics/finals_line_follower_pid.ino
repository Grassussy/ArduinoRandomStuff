// pins for the ir sensors
const int pinIRLeft = A0;
const int pinIRMiddle = A1;
const int pinIRRight = A2;

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
float kp = 8;
float ki = 0.0001;
float kd = 2;

// values passed to the PID controller
int p = 1;
int i = 0;
int d = 0;

// values used for computing the derivative
int error = 0;
int lastError = 0;

// constants for the motors speed
const int minSpeed = -150;
const int maxSpeed = 150;
const int baseSpeed = 100;

const int sensorCount = 3;
int sensorValues[sensorCount];

void setup() {
  pinMode(pinIRLeft, INPUT);
  pinMode(pinIRMiddle, INPUT);
  pinMode(pinIRRight, INPUT);

  pinMode(pinMotorLA, OUTPUT);
  pinMode(pinMotorLB, OUTPUT);
  pinMode(pinMotorRA, OUTPUT);
  pinMode(pinMotorRB, OUTPUT);

  pinMode(pinMotorLEnable, OUTPUT);
  pinMode(pinMotorREnable, OUTPUT);
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
    if (sensorValues[i] < 0) {
      sensorValues[i] = 0;
    } else if (sensorValues[i] > 1000) {
      sensorValues[i] = 1000;
    }
    sensorAverage += sensorValues[i] * i * 1000;
    sensorSum += sensorValues[i];
  }

  return map(float(sensorAverage / sensorSum), 0, 2000, -50, 50);
}

void pidControl(float kp, float ki, float kd) {
  error = computeError();

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
  // motorLSpeed = -motorLSpeed;
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