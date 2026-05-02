/*
  PROJE: Engel Algılama Robotu (Gelişmiş - Yan Sensör Düzeltmesi)
  AÇIKLAMA: 3 ultrasonik sensör (ön, sol, sağ) ile daha hassas engel kaçınma
  Yan sensörler kullanılarak robot düz yolda kalabilir ve yan engelleri algılayabilir
  Hız trim ayarları ve yan sensör düzeltmesi ile iyileştirilen versiyon
  KULLANILAN BILEŞENLER: 3x HC-SR04 sensör, 2x DC motor
  NOTLAR: Geliştirilmiş engel kaçınma algoritması
*/

//
// ---------------- KOLAY AYARLAR ----------------
//

#define BASE_SPEED 110

#define LEFT_TRIM 0
#define RIGHT_TRIM 50

#define STOP_DISTANCE 35

#define SIDE_K 2.0
#define SIDE_MAX_CORRECTION 50
#define SIDE_TOLERANCE_CM 2.0

#define MAX_TOTAL_CORRECTION 45

// Düzeltme ters çalışırsa bunu -1 yap
#define SIDE_CORRECTION_SIGN -1

#define SIDE_CALIBRATION_SAMPLE 10

//
// ---------------- PIN TANIMLARI ----------------
//

#define FAKE_5V 53

#define TRIG_PIN 26
#define ECHO_PIN 27

#define TRIG_L_PIN 28
#define ECHO_L_PIN 29

#define TRIG_R_PIN 30
#define ECHO_R_PIN 31

#define LEFT_PWM 2
#define LEFT_IN1 23
#define LEFT_IN2 22

#define RIGHT_PWM 3
#define RIGHT_IN1 24
#define RIGHT_IN2 25

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
// ---------------- KORİDOR KALİBRASYONU ----------------
//

void calibrateSideDistances() {
  Serial.println("Sag-sol mesafe kalibrasyonu basliyor...");
  stopMotors();
  delay(500);

  targetLeftDistance = readAverageDistanceCM(TRIG_L_PIN, ECHO_L_PIN, SIDE_CALIBRATION_SAMPLE);
  targetRightDistance = readAverageDistanceCM(TRIG_R_PIN, ECHO_R_PIN, SIDE_CALIBRATION_SAMPLE);

  Serial.print("Sol hedef: ");
  Serial.print(targetLeftDistance);
  Serial.print(" cm | Sag hedef: ");
  Serial.print(targetRightDistance);
  Serial.println(" cm");
}

//
// ---------------- SAHTE PID / MESAFE DÜZELTME ----------------
//

int calculateSideCorrection(float leftDistance, float rightDistance) {
  float leftError = targetLeftDistance - leftDistance;
  float rightError = rightDistance - targetRightDistance;

  float totalError = leftError + rightError;

  if (fabs(totalError) < SIDE_TOLERANCE_CM) {
    totalError = 0;
  }

  int correction = totalError * SIDE_K * SIDE_CORRECTION_SIGN;

  correction = constrain(correction, -SIDE_MAX_CORRECTION, SIDE_MAX_CORRECTION);

  return correction;
}

//
// ---------------- ENGEL RUTİNİ ----------------
//

void handleObstacle() {
  Serial.println("Engel algilandi -> Dur");
  stopMotors();

  while (readDistanceCM(TRIG_PIN, ECHO_PIN) <= STOP_DISTANCE) {
    delay(50);
  }

  Serial.println("Engel kalkti, devam.");
}

//
// ---------------- SETUP ----------------
//

void setup() {
  Serial.begin(115200);

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

  Serial.println("Araci koridorun ortasina koy.");
  delay(2000);

  calibrateSideDistances();

  Serial.println("Hazir.");
}

//
// ---------------- LOOP ----------------
//

void loop() {
  float frontDistance = readDistanceCM(TRIG_PIN, ECHO_PIN);

  if (frontDistance <= STOP_DISTANCE) {
    handleObstacle();
    return;
  }

  float leftDistance = readDistanceCM(TRIG_L_PIN, ECHO_L_PIN);
  float rightDistance = readDistanceCM(TRIG_R_PIN, ECHO_R_PIN);

  int sideCorrection = calculateSideCorrection(leftDistance, rightDistance);

  int totalCorrection = constrain(sideCorrection, -MAX_TOTAL_CORRECTION, MAX_TOTAL_CORRECTION);

  int leftSpeed = BASE_SPEED + LEFT_TRIM - totalCorrection;
  int rightSpeed = BASE_SPEED + RIGHT_TRIM + totalCorrection;

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

  Serial.print(" | Corr: ");
  Serial.print(totalCorrection);

  Serial.print(" | L: ");
  Serial.print(leftSpeed);

  Serial.print(" | R: ");
  Serial.println(rightSpeed);

  delay(40);
}
