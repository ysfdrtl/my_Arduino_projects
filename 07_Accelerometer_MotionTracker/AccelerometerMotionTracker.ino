/*
  PROJE: İvmeölçer Hareketi İzleme
  AÇIKLAMA: MMA7660 3 eksenli ivmeölçer sensörü ile hareketin izlenmesi
  Sensörün İletişim filtreleme ve kalibrasyonu yaparak ivme, hız ve konum verilerini hesaplar
  KULLANILAN BILEŞENLER: MMA7660 accelerometer (I2C)
  NOTLAR: Sürekli kalibrasyonlar yapıldığında en iyi sonuçlar alınır
*/

#include <Wire.h>
#include "MMA7660.h"
#include <math.h>

MMA7660 accel;

// Filtre
float axFiltered = 0.0;
const float alpha = 0.85;   // 0.8 - 0.95 arası denenebilir

// Kalibrasyon
float axOffset = 0.0;
bool calibrated = false;

// Zaman
unsigned long lastMicros = 0;

// Durum değişkenleri
float velocityX = 0.0;   // m/s
float positionX = 0.0;   // m

// Ayarlar
const float gToMs2 = 9.81;
const float accelDeadband = 0.03;   // g cinsinden; küçük gürültüyü yok say
const float velocityDeadband = 0.02; // m/s altını sıfırla
const float maxDt = 0.05;           // çok büyük dt olursa yok say

void setup() {
  Serial.begin(115200);
  Wire.begin();
  accel.init();

  Serial.println("Sensoru sabit tut, kalibrasyon yapiliyor...");
  delay(2000);

  // Başlangıç ofset kalibrasyonu
  float sum = 0.0;
  const int samples = 200;

  for (int i = 0; i < samples; i++) {
    float ax, ay, az;
    accel.getAcceleration(&ax, &ay, &az);
    sum += ax;
    delay(5);
  }

  axOffset = sum / samples;
  calibrated = true;

  lastMicros = micros();

  Serial.println("Kalibrasyon tamam.");
  Serial.println("Plotter: x_cm hiz_cm_s ivme");
}

void loop() {
  if (!calibrated) return;

  unsigned long now = micros();
  float dt = (now - lastMicros) / 1000000.0;
  lastMicros = now;

  if (dt <= 0 || dt > maxDt) return;

  float ax, ay, az;
  accel.getAcceleration(&ax, &ay, &az);

  // Ofset çıkar
  float axRelative = ax - axOffset;

  // Low-pass filtre
  axFiltered = alpha * axFiltered + (1.0 - alpha) * axRelative;

  // Deadband
  if (fabs(axFiltered) < accelDeadband) {
    axFiltered = 0.0;
  }

  // g -> m/s^2
  float axMs2 = axFiltered * gToMs2;

  // İntegrasyon: ivme -> hız
  velocityX += axMs2 * dt;

  // Çok küçük hızları sıfırla
  if (fabs(velocityX) < velocityDeadband && axFiltered == 0.0) {
    velocityX = 0.0;
  }

  // İntegrasyon: hız -> konum
  positionX += velocityX * dt;

  // cm'ye çevir
  float positionCm = positionX * 100.0;
  float velocityCmS = velocityX * 100.0;

  // Serial Plotter için 3 çizgi
  Serial.print(positionCm);
  Serial.print(" ");
  Serial.print(velocityCmS);
  Serial.print(" ");
  Serial.println(axFiltered);

  delay(10);
}
