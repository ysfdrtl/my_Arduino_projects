# 🤖 Engel Algılama Robotu (Gelişmiş - Yan Sensör Düzeltmesi) - Bağlantı Şeması

## 📌 Genel Bilgi
Bu proje 3 ultrasonik sensör (ön, sol, sağ), 2 DC motor ve L298N motor sürücüsü kullanmaktadır. Yan sensör düzeltmesi ile robot düz yolda kalmayı başarır. Önceki versiyonlardan daha gelişmiş engel kaçınma algoritmasına sahiptir.

## 📋 Pin Bağlantıları

### Ultrasonik Sensörler (HC-SR04)

| Sensör | Trig Pin | Echo Pin | Pozisyon | Kullanım |
|--------|----------|----------|----------|----------|
| Ön Sensör | 26 | 27 | İleri | Engel algılama |
| Sol Sensör | 28 | 29 | Sol taraf | Yön düzeltmesi |
| Sağ Sensör | 30 | 31 | Sağ taraf | Yön düzeltmesi |

### DC Motor Kontrol (L298N)

| Motor | PWM Pin | IN1 Pin | IN2 Pin | Pozisyon |
|-------|---------|---------|---------|----------|
| Sol Motor | 2 | 23 | 22 | Sol tekerlek |
| Sağ Motor | 3 | 24 | 25 | Sağ tekerlek |

### Diğer Bağlantılar

| Bileşen | Arduino Pin | Açıklama |
|---------|------------|---------|
| Fake 5V | 53 | Yardımcı beslenme |
| GND | GND | Toprak |
| +5V | +5V | Ana beslenme |

## 🔌 Fiziksel Bağlantı Diyagramı

### Arduino Mega Pinleri

```
ARDUINO MEGA 2560 - PIN GÖREV HARITASI
┌─────────────────────────────────────────────┐
│                                             │
│  ULTRASONIK SENSÖR PİNLERİ                  │
│  ├─ PIN 26 → HC-SR04 Ön Sensörü (Trig)      │
│  ├─ PIN 27 → HC-SR04 Ön Sensörü (Echo)      │
│  ├─ PIN 28 → HC-SR04 Sol Sensörü (Trig)     │
│  ├─ PIN 29 → HC-SR04 Sol Sensörü (Echo)     │
│  ├─ PIN 30 → HC-SR04 Sağ Sensörü (Trig)     │
│  └─ PIN 31 → HC-SR04 Sağ Sensörü (Echo)     │
│                                             │
│  MOTOR KONTROL PİNLERİ (L298N)              │
│  ├─ PWM 2  → EN_A (Sol Motor Hız)           │
│  ├─ PIN 23 → IN1 (Sol Motor Yön)            │
│  ├─ PIN 22 → IN2 (Sol Motor Yön)            │
│  ├─ PWM 3  → EN_B (Sağ Motor Hız)           │
│  ├─ PIN 24 → IN3 (Sağ Motor Yön)            │
│  └─ PIN 25 → IN4 (Sağ Motor Yön)            │
│                                             │
│  I2C HABERLEŞME (Accelerometer)             │
│  ├─ PIN 20 → SDA (MMA7660)                  │
│  └─ PIN 21 → SCL (MMA7660)                  │
│                                             │
│  BESLENME & TOPRAK                          │
│  ├─ +5V  → Sensörler & L298N                │
│  └─ GND → Tüm Komponentler                  │
│                                             │
└─────────────────────────────────────────────┘
```

### Blok Bağlantı Diyagramı

```
┌─────────────────────────────────────────────┐
│          ARDUINO MEGA 2560                  │
│  ┌──────────────────────────────────┐       │
│  │  Sensor Pinleri | Motor Pinleri  │       │
│  │ ───────────────────────────────  │       │
│  │ 26,27 (Ön Sens.)  2,3 (PWM)      │       │
│  │ 28,29 (Sol Sens.) 22-25 (Yön)    │       │
│  │ 30,31 (Sağ Sens.) 20,21 (I2C)    │       │
│  └──────────────────────────────────┘       │
└─────────────┬──────────────────┬────────────┘
              │                  │
         ┌────┴──────┐    ┌──────┴─────┐
         │ SENSÖRLER │    │ L298N      │
         │ (HC-SR04) │    │ Motor      │
         │ x3        │    │ Sürücü     │
         │           │    │            │
         │ + MMA7660 │    │ OUT1,OUT2  │
         │ (I2C)     │    │ OUT3,OUT4  │
         └───────────┘    └──────┬─────┘
                                 │
                    ┌────────────┴────────────┐
                    │                         │
                 ┌──┴──┐                   ┌──┴──┐
                 │Sol  │                   │Sağ  │
                 │Motor│                   │Motor│
                 └─────┘                   └─────┘
```

### Pinleme Özeti Tablosu

**HC-SR04 Sensörler:**
```
Ön Sensör  → Trig: 26, Echo: 27 (VCC: +5V, GND: GND)
Sol Sensör → Trig: 28, Echo: 29 (VCC: +5V, GND: GND)
Sağ Sensör → Trig: 30, Echo: 31 (VCC: +5V, GND: GND)
```

**L298N Motor Sürücü:**
```
Sol Motor  → EN_A: PWM2, IN1: 23, IN2: 22 → OUT1, OUT2
Sağ Motor  → EN_B: PWM3, IN3: 24, IN4: 25 → OUT3, OUT4
Beslenme   → +5V (Arduino), GND (Arduino)
```

**MMA7660 Accelerometer (I2C):**
```
SDA: PIN 20, SCL: PIN 21 (VCC: +5V, GND: GND)
```

## ⚡ Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Arduino Board | Mega 2560 |
| Ultrasonik Sensör | HC-SR04 x3 |
| Motor Sürücü | L298N |
| Motor Voltajı | 5-12V |
| PWM Frekansı | 490-980 Hz |
| Temel Hız | 110 |
| Dönüş Hızı | 130 |
| Engel Mesafesi | 35 cm |
| Dönüş Zamanı | 1000 ms |
| Durdurma Zamanı | 150 ms |
| Yan Sensor K Değeri | 2.0 |
| Max Düzeltme | 50 (Yan), 45 (Toplam) |
| Yan Tolerans | 2.0 cm |

## 🔧 Gelişmiş Özellikler

### Yan Sensör Düzeltmesi (Side Sensor Correction)
- Sol ve sağ sensörler arasındaki mesafe farkı hesaplanır
- Motor hızları fark oranına göre düzeltilir
- Robot düz yolda kalmayı başarır

### Hız Trim Ayarları
```
LEFT_TRIM = 0
RIGHT_TRIM = 50
```
- Sol motor ile sağ motor arasındaki hız farkını ayarlar

### Düzeltme Algoritması
```
fark = sol_mesafe - sağ_mesafe
düzeltme = fark * SIDE_K  // SIDE_K = 2.0
```

## 🔧 Bağlantı Adımları

1. **Üç HC-SR04 Sensörünü Bağlayın:**
   - Ön: Trig→26, Echo→27
   - Sol: Trig→28, Echo→29
   - Sağ: Trig→30, Echo→31
   - Tümü: VCC→+5V, GND→GND

2. **L298N Motor Sürücüsünü Bağlayın:**
   - EN_A→PWM 2, IN1→23, IN2→22
   - EN_B→PWM 3, IN3→24, IN4→25

3. **DC Motorları Bağlayın:**
   - Sol Motor: L298N OUT1 & OUT2
   - Sağ Motor: L298N OUT3 & OUT4

4. **Beslenmeyi Sağlayın:**
   - Arduino: USB
   - L298N: +5V (Arduino) ve GND
   - Motorlar: Harici 5-12V kaynağı (önerilir)

## ⚠️ Önemli Notlar

- Yan sensörlerin kalibrasyon mesafesi belirlenmelidir
- Motor hız trim değerlerini ortama göre ayarlayın
- HC-SR04 sensörleri 15-45 cm aralığında çalışır
- Düzeltme ters çalışırsa `SIDE_CORRECTION_SIGN = -1` yapın
- Yan tolerans değeri çok küçükse robotun çok sıkça düzeltme yapmasına neden olur

## 🎯 Beklenen Sonuç

Robot engelleri algılayarak onlardan kaçınacak ve yan sensörler sayesinde düz yolda kalmaya çalışacaktır. Bu versiyon önceki versiyonlara kıyasla daha kararlı ve kontrollü bir hareket sergiler.

## 📊 Parametre Ayarları

Donanım ve ortama göre şu değerleri deneyebilirsiniz:

```
BASE_SPEED: 100-150 (temel hız)
TURN_SPEED: 120-160 (dönüş hızı)
STOP_DISTANCE: 30-40 (engel mesafesi)
SIDE_K: 1.5-3.0 (düzeltme katsayısı)
SIDE_MAX_CORRECTION: 30-60 (max düzeltme)
```
