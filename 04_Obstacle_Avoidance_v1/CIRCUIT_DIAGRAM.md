# 🤖 Engel Algılama Robotu v1 - Bağlantı Şeması

## 📌 Genel Bilgi
Bu proje 1 ultrasonik sensör (ön), 2 DC motor, MMA7660 ivmeölçer ve L298N motor sürücüsü kullanmaktadır. Temel engel algılama ve kaçınma robotunun v1 versiyonudur.

## 📋 Pin Bağlantıları

### Ultrasonik Sensör (HC-SR04)

| Sensör | Trig Pin | Echo Pin | Pozisyon |
|--------|----------|----------|----------|
| Ön Sensör | 7 | 6 | İleri |

### DC Motor Kontrol (L298N)

| Motor | PWM Pin | IN1 Pin | IN2 Pin | Pozisyon |
|-------|---------|---------|---------|----------|
| Sol Motor | 5 | 8 | 9 | Sol tekerlek |
| Sağ Motor | 10 | 11 | 12 | Sağ tekerlek |

### İvmeölçer Sensör (MMA7660)

| Bileşen | Arduino Pin | Açıklama |
|---------|------------|---------|
| SDA | PIN 20 | I2C Veri hattı |
| SCL | PIN 21 | I2C Saat hattı |
| VCC | +5V | Beslenme |
| GND | GND | Toprak |

## 🔌 Fiziksel Bağlantı Diyagramı

```
Arduino Mega/Uno
┌──────────────────────┐
│                      │
│ 7 (Trig Ön)     ─────┼───────────── HC-SR04 Trig
│ 6 (Echo Ön)     ─────┼───────────── HC-SR04 Echo
│                      │
│ PWM 5 (EN_A)    ─────┼───────────── L298N EN_A
│ 8 (IN1)         ─────┼───────────── L298N IN1
│ 9 (IN2)         ─────┼───────────── L298N IN2
│                      │
│ PWM 10 (EN_B)   ─────┼───────────── L298N EN_B
│ 11 (IN3)        ─────┼───────────── L298N IN3
│ 12 (IN4)        ─────┼───────────── L298N IN4
│                      │
│ 20 (SDA)        ─────┼───────────── MMA7660 SDA
│ 21 (SCL)        ─────┼───────────── MMA7660 SCL
│                      │
│ +5V ───────────┬─────┼───────────── +5V
│ GND ───────────┼─────┼───────────── GND
└──────────────────────┘
                 │
        ┌────────┴────────────────┐
        │   L298N Motor Sürücü    │
        │                         │
        │ EN_A, IN1, IN2 ◄────────┤
        │ EN_B, IN3, IN4 ◄────────┤
        │                         │
        │ OUT1, OUT2 ──┐          │
        │ (Sol Motor)  │          │
        │              │          │
        │ OUT3, OUT4 ──┤          │
        │ (Sağ Motor)  │          │
        │              │          │
        └──────────────┴──────────┘

HC-SR04 Sensör:
Trig: 7, Echo: 6
VCC: +5V, GND: GND

MMA7660 Accelerometer (I2C Bus):
SDA: 20, SCL: 21
VCC: +5V, GND: GND
```

## ⚡ Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Arduino Board | Mega 2560 veya Uno |
| Motor Sürücü | L298N |
| Motor Voltajı | 5-12V |
| PWM Frekansı | 490-980 Hz |
| HC-SR04 Voltajı | 5V |
| MMA7660 Protokol | I2C (400 kHz) |
| Temel Hız | 150 |
| Dönüş Hızı | 140 |
| Engel Mesafesi | 20 cm |
| İvme Filtresi | 0.90 (alpha) |

## 🔧 Motor Kontrol Mantığı

### İleri Hareketi
- Sol Motor: PWM5 ile hız, IN1=HIGH, IN2=LOW
- Sağ Motor: PWM10 ile hız, IN3=HIGH, IN4=LOW

### Engel Algılaması
- Ön sensör 20 cm'den yakın mesafe algılarsa dönüş yapılır
- İvmeölçer ile sabit ileri gitme sağlanır

### Dönüş Mantığı
- Engel algılandığında motor hızı 140'a ayarlanır
- Belirli süre dönüş yapılır
- Sonra tekrar ileri gidişe başlanır

## 🔧 Bağlantı Adımları

1. **HC-SR04 Sensörünü Bağlayın:**
   - Trig→7, Echo→6
   - VCC→+5V, GND→GND

2. **L298N Motor Sürücüsünü Bağlayın:**
   - EN_A→PWM 5, IN1→8, IN2→9
   - EN_B→PWM 10, IN3→11, IN4→12
   - +5V→Arduino +5V, GND→Arduino GND

3. **DC Motorları Bağlayın:**
   - Sol Motor: OUT1 & OUT2
   - Sağ Motor: OUT3 & OUT4

4. **MMA7660 Accelerometer Bağlayın (I2C):**
   - SDA→20, SCL→21
   - VCC→+5V, GND→GND

5. **Beslenmeyi Sağlayın:**
   - Arduino: USB veya +5V
   - Motorlar: Harici 5-12V kaynağı (önerilir)

## ⚠️ Önemli Notlar

- HC-SR04 15-45 cm aralığında en iyi çalışır
- MMA7660 I2C haberleşme kullanır
- Motor sürücü yeterli akım sağlamak için harici beslenme gerekebilir
- İvmeölçer filtresi başlangıçta kalibre edilir

## 🎯 Beklenen Sonuç

Robot ön taraftaki engelleri algılayarak onlardan kaçınacak ve otonom olarak hareket edecektir. İvmeölçer sayesinde hareket daha kararlı olacaktır.
