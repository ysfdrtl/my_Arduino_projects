/*
  PROJE: Engel Algılama Robotu v1 (Temel)
  AÇIKLAMA: 1 ultrasonik sensör (ön) ve 2 DC motor ile hareket eden otonom robot
  Engelleri algılayarak onlardan kaçınarak ilerlemeye çalışır
  İvmeölçer sensörü (MMA7660) ile hareket düzeltmesi yapar
  KULLANILAN BILEŞENLER: HC-SR04 sensör, 2x DC motor, MMA7660 accelerometer
  NOTLAR: v2'nin temel versiyonu
*/

#include <Wire.h>
#include "MMA7660.h"
#include <math.h>

//
// ---------------- PIN TANIMLARI ----------------
//

// HC-SR04
#define TRIG_PIN 7
#define ECHO_PIN 6

// Sol motor
#define LEFT_PWM 5
#define LEFT_IN1 8
#define LEFT_IN2 9

// Sağ motor
#define RIGHT_PWM 10
#define RIGHT_IN1 11
#define RIGHT_IN2 12

//
// ---------------- AYARLAR ----------------
//

#define BASE_SPEED 150
#define TURN_SPEED 140
#define MAX_CORRECTION 35
#define STOP_DISTANCE 20

#define ACCEL_DEADBAND 0.03
#define ALPHA 0.90

#define TURN_TIME_MS 1000
#define STOP_AFTER_TURN_MS 150
#define CALIBRATION_SAMPLES 100

MMA7660 accel;

float axFiltered = 0.0;
float axOffset = 0.0;

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
  // Sol ileri, sağ geri -> yerinde sağa dönüş
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

float readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(3);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return 999.0;
  }

  return duration * 0.0343 / 2.0;
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

int calculateCorrection(float ax) {
  float k = 120.0;
  int correction = (int)(ax * k);
  correction = constrain(correction, -MAX_CORRECTION, MAX_CORRECTION);
  return correction;
}

//
// ---------------- ENGEL RUTİNİ ----------------
//

void handleObstacle() {
  Serial.println("Engel algilandi -> Dur");
  stopMotors();
  delay(200);

  Serial.println("Saga donuluyor...");
  turnRightInPlace(TURN_SPEED);
  delay(TURN_TIME_MS);

  stopMotors();
  delay(STOP_AFTER_TURN_MS);

  Serial.println("Ivmeolcer yeniden kalibre ediliyor...");
  calibrateAccel();

  Serial.println("Kalibrasyon tamam, ileri gidiliyor.");
}

//
// ---------------- SETUP / LOOP ----------------
//

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

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
  Serial.println("Hazir.");
}

void loop() {
  float distance = readDistanceCM();

  if (distance <= STOP_DISTANCE) {
    handleObstacle();
    return;
  }

  float ax = readAccelXFiltered();
  int correction = calculateCorrection(ax);

  int leftSpeed = BASE_SPEED - correction;
  int rightSpeed = BASE_SPEED + correction;

  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  moveForward(leftSpeed, rightSpeed);

  Serial.print("Mesafe: ");
  Serial.print(distance);
  Serial.print(" cm | ax: ");
  Serial.print(ax, 4);
  Serial.print(" | correction: ");
  Serial.print(correction);
  Serial.print(" | L: ");
  Serial.print(leftSpeed);
  Serial.print(" | R: ");
  Serial.println(rightSpeed);

  delay(20);
}
