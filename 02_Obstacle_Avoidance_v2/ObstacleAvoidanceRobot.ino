/*
  PROJE: Engel Algılama Robotu v2 (Gelişmiş)
  AÇIKLAMA: 3 ultrasonik sensör (ön, sol, sağ) ve 2 DC motor ile hareket eden otonom robot
  Engelleri algılayarak onlardan kaçınarak ilerlemeye çalışır
  İvmeölçer sensörü (MMA7660) ile ileri hareketi stabilize eder
  KULLANILAN BILEŞENLER: 3x HC-SR04 sensör, 2x DC motor, MMA7660 accelerometer
  NOTLAR: Yan sensörlerden kaynaklanan hataları düzeltir
*/

//
// ---------------- PIN TANIMLARI ----------------
//
#define FAKE_5V 53

// Ön HC-SR04
#define TRIG_PIN 26
#define ECHO_PIN 27

// Sol HC-SR04
#define TRIG_L_PIN 28
#define ECHO_L_PIN 29

// Sağ HC-SR04
#define TRIG_R_PIN 30
#define ECHO_R_PIN 31

// Sol motor
#define LEFT_PWM 2
#define LEFT_IN1 23
#define LEFT_IN2 22

// Sağ motor
#define RIGHT_PWM 3
#define RIGHT_IN1 24
#define RIGHT_IN2 25

#include <Wire.h>
#include "MMA7660.h"
#include <math.h>

//
// ---------------- AYARLAR ----------------
//

#define BASE_SPEED 110
#define TURN_SPEED 130
#define STOP_DISTANCE 35

#define TURN_TIME_MS 1000
#define STOP_AFTER_TURN_MS 150

#define CALIBRATION_SAMPLES 100

// İvme filtresi
#define ALPHA 0.92
#define ACCEL_DEADBAND 0.04
#define ACCEL_K 35.0

// Yan sensör düzeltmesi
#define SIDE_K 3.0
#define SIDE_MAX_CORRECTION 35
#define SIDE_TOLERANCE_CM 2.0

// Genel düzeltme limiti
#define MAX_TOTAL_CORRECTION 45

// Motor trim
#define MOTOR_TRIM -30

// Yön ters çalışırsa -1 yap
#define SIDE_CORRECTION_SIGN -1
#define ACCEL_CORRECTION_SIGN 1

MMA7660 accel;

float axFiltered = 0.0;
float axOffset = 0.0;

float targetLeftDistance = 0.0;
float targetRightDistance = 0.0;

//
// ---------------- MOTOR FONKSİYONLARI ----------------
//

void setLeftMotor(int speedValue) {
  speedValue = constrain(speedValue, -255, 255);

  if (speedValue > 0) {
    digitalWrite(LEFT_IN1, HIGH);
    digitalWrite(LEFT_IN2, LOW);
    analogWrite(LEFT_PWM, speedValue);
  } else if (speedValue < 0) {
    digitalWrite(LEFT_IN1, LOW);
    digitalWrite(LEFT_IN2, HIGH);
    analogWrite(LEFT_PWM, -speedValue);
  } else {
    digitalWrite(LEFT_IN1, LOW);
    digitalWrite(LEFT_IN2, LOW);
    analogWrite(LEFT_PWM, 0);
  }
}

void setRightMotor(int speedValue) {
  speedValue = constrain(speedValue, -255, 255);

  if (speedValue > 0) {
    digitalWrite(RIGHT_IN1, HIGH);
    digitalWrite(RIGHT_IN2, LOW);
    analogWrite(RIGHT_PWM, speedValue);
  } else if (speedValue < 0) {
    digitalWrite(RIGHT_IN1, LOW);
    digitalWrite(RIGHT_IN2, HIGH);
    analogWrite(RIGHT_PWM, -speedValue);
  } else {
    digitalWrite(RIGHT_IN1, LOW);
    digitalWrite(RIGHT_IN2, LOW);
    analogWrite(RIGHT_PWM, 0);
  }
}

void moveForward(int leftSpeed, int rightSpeed) {
  setLeftMotor(leftSpeed);
  setRightMotor(rightSpeed);
}

void turnRightInPlace(int speedValue) {
  setLeftMotor(speedValue);
  setRightMotor(-speedValue);
}

void stopMotors() {
  setLeftMotor(0);
  setRightMotor(0);
}

//
// ---------------- MESAFE ÖLÇÜMÜ ----------------
//

float readDistanceCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return 999.0;
  }

  return duration * 0.0343 / 2.0;
}

float readAverageDistanceCM(int trigPin, int echoPin, int sampleCount) {
  float sum = 0.0;
  int validCount = 0;

  for (int i = 0; i < sampleCount; i++) {
    float d = readDistanceCM(trigPin, echoPin);

    if (d > 2 && d < 400) {
      sum += d;
      validCount++;
    }

    delay(20);
  }

  if (validCount == 0) {
    return 999.0;
  }

  return sum / validCount;
}

//
// ---------------- BAŞLANGIÇ KORİDOR KALİBRASYONU ----------------
//

void calibrateSideDistances() {
  Serial.println("Sag-sol mesafe kalibrasyonu basliyor...");
  stopMotors();
  delay(500);

  targetLeftDistance = readAverageDistanceCM(TRIG_L_PIN, ECHO_L_PIN, 10);
  targetRightDistance = readAverageDistanceCM(TRIG_R_PIN, ECHO_R_PIN, 10);

  Serial.print("Sol hedef mesafe: ");
  Serial.print(targetLeftDistance);
  Serial.print(" cm | Sag hedef mesafe: ");
  Serial.print(targetRightDistance);
  Serial.println(" cm");
}

//
// ---------------- İVMEÖLÇER ----------------
//

void calibrateAccel() {
  float sum = 0.0;
  axFiltered = 0.0;

  for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
    float ax, ay, az;
    accel.getAcceleration(&ax, &ay, &az);
    sum += ax;
    delay(5);
  }

  axOffset = sum / CALIBRATION_SAMPLES;
}

float readAccelXFiltered() {
  float ax, ay, az;
  accel.getAcceleration(&ax, &ay, &az);

  float axRelative = ax - axOffset;
  axFiltered = ALPHA * axFiltered + (1.0 - ALPHA) * axRelative;

  if (fabs(axFiltered) < ACCEL_DEADBAND) {
    axFiltered = 0.0;
  }

  return axFiltered;
}

int calculateAccelCorrection(float ax) {
  int correction = (int)(ax * ACCEL_K * ACCEL_CORRECTION_SIGN);
  return constrain(correction, -20, 20);
}

//
// ---------------- YAN SENSÖR KORİDOR KONTROLÜ ----------------
//

int calculateSideCorrection(float leftDistance, float rightDistance) {
  float leftError = targetLeftDistance - leftDistance;
  float rightError = rightDistance - targetRightDistance;

  float totalError = leftError + rightError;

  if (fabs(totalError) < SIDE_TOLERANCE_CM) {
    totalError = 0;
  }

  int correction = (int)(totalError * SIDE_K * SIDE_CORRECTION_SIGN);
  return constrain(correction, -SIDE_MAX_CORRECTION, SIDE_MAX_CORRECTION);
}

//
// ---------------- ENGEL RUTİNİ ----------------
//

void handleObstacle() {
  Serial.println("Engel algilandi -> Dur");
  stopMotors();

  // Engel kalkana kadar bekle
  while (readDistanceCM(TRIG_PIN, ECHO_PIN) <= STOP_DISTANCE) {
    delay(50);
  }

  Serial.println("Engel kalkti, ileri devam.");
}

//
// ---------------- SETUP / LOOP ----------------
//

void setup() {
  Serial.begin(115200);
  Wire.begin();
  pinMode(FAKE_5V, OUTPUT);
  digitalWrite(FAKE_5V, HIGH);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(TRIG_L_PIN, OUTPUT);
  pinMode(ECHO_L_PIN, INPUT);

  pinMode(TRIG_R_PIN, OUTPUT);
  pinMode(ECHO_R_PIN, INPUT);

  pinMode(LEFT_PWM, OUTPUT);
  pinMode(LEFT_IN1, OUTPUT);
  pinMode(LEFT_IN2, OUTPUT);

  pinMode(RIGHT_PWM, OUTPUT);
  pinMode(RIGHT_IN1, OUTPUT);
  pinMode(RIGHT_IN2, OUTPUT);

  stopMotors();

  accel.init();

  Serial.println("Baslangic kalibrasyonu, araci sabit tut...");
  delay(2000);

  calibrateAccel();
  calibrateSideDistances();

  Serial.println("Hazir.");
}

void loop() {
  float frontDistance = readDistanceCM(TRIG_PIN, ECHO_PIN);

  if (frontDistance <= STOP_DISTANCE) {
    handleObstacle();
    return;
  }

  float leftDistance = readDistanceCM(TRIG_L_PIN, ECHO_L_PIN);
  float rightDistance = readDistanceCM(TRIG_R_PIN, ECHO_R_PIN);

  float ax = readAccelXFiltered();

  int accelCorrection = calculateAccelCorrection(ax);
  int sideCorrection = calculateSideCorrection(leftDistance, rightDistance);

  int totalCorrection = sideCorrection + accelCorrection;
  totalCorrection = constrain(totalCorrection, -MAX_TOTAL_CORRECTION, MAX_TOTAL_CORRECTION);

  int leftSpeed = BASE_SPEED - MOTOR_TRIM - totalCorrection;
  int rightSpeed = BASE_SPEED + MOTOR_TRIM + totalCorrection;

  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  moveForward(leftSpeed, rightSpeed);

  Serial.print("On: ");
  Serial.print(frontDistance);

  Serial.print(" | Sol: ");
  Serial.print(leftDistance);

  Serial.print(" | Sag: ");
  Serial.print(rightDistance);

  Serial.print(" | Hedef Sol: ");
  Serial.print(targetLeftDistance);

  Serial.print(" | Hedef Sag: ");
  Serial.print(targetRightDistance);

  Serial.print(" | sideCorr: ");
  Serial.print(sideCorrection);

  Serial.print(" | accCorr: ");
  Serial.print(accelCorrection);

  Serial.print(" | totalCorr: ");
  Serial.print(totalCorrection);

  Serial.print(" | L: ");
  Serial.print(leftSpeed);

  Serial.print(" | R: ");
  Serial.println(rightSpeed);

  delay(40);
}
