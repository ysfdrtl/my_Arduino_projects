# 🤖 Engel Algılama Robotu v2 - Bağlantı Şeması

## 📌 Genel Bilgi
Bu proje 3 ultrasonik sensör (ön, sol, sağ), 2 DC motor, MMA7660 ivmeölçer ve L298N motor sürücüsü kullanmaktadır. Engelleri algılayarak onlardan kaçınan otonom bir robot yapısı.

## 📋 Pin Bağlantıları

### Ultrasonik Sensörler (HC-SR04)

| Sensör | Trig Pin | Echo Pin | Pozisyon |
|--------|----------|----------|----------|
| Ön Sensör | 26 | 27 | İleri |
| Sol Sensör | 28 | 29 | Sol taraf |
| Sağ Sensör | 30 | 31 | Sağ taraf |

### DC Motor Kontrol (L298N)

| Motor | PWM Pin | IN1 Pin | IN2 Pin | Pozisyon |
|-------|---------|---------|---------|----------|
| Sol Motor | 2 | 23 | 22 | Sol tekerlek |
| Sağ Motor | 3 | 24 | 25 | Sağ tekerlek |

### İvmeölçer Sensör (MMA7660)

| Bileşen | Arduino Pin | Açıklama |
|---------|------------|---------|
| SDA | PIN 20 | I2C Veri hattı |
| SCL | PIN 21 | I2C Saat hattı |
| VCC | +5V | Beslenme |
| GND | GND | Toprak |

### Diğer Bağlantılar

| Bileşen | Arduino Pin | Açıklama |
|---------|------------|---------|
| Fake 5V | 53 | Yardımcı beslenme pini |
| GND | GND | Toprak |
| +5V | +5V | Ana beslenme |

## 🔌 Fiziksel Bağlantı Diyagramı

```
Arduino Mega 2560                    L298N Motor Sürücü
┌──────────────────────┐            ┌──────────────┐
│                      │            │              │
│ 26 (Trig Ön)    ─────┼────────────┤ Ön HC-SR04   │
│ 27 (Echo Ön)    ─────┼────────────┤ (Trig: 26)   │
│                      │            │ (Echo: 27)   │
│ 28 (Trig Sol)   ─────┼────────────┤ Sol HC-SR04  │
│ 29 (Echo Sol)   ─────┼────────────┤ (Trig: 28)   │
│                      │            │ (Echo: 29)   │
│ 30 (Trig Sağ)   ─────┼────────────┤ Sağ HC-SR04  │
│ 31 (Echo Sağ)   ─────┼────────────┤ (Trig: 30)   │
│                      │            │ (Echo: 31)   │
│ PWM 2           ─────┼────────────┤ EN_A         │
│ PIN 23 (IN1)    ─────┼────────────┤ IN1          │
│ PIN 22 (IN2)    ─────┼────────────┤ IN2          │
│                      │            │              │
│ PWM 3           ─────┼────────────┤ EN_B         │
│ PIN 24 (IN3)    ─────┼────────────┤ IN3          │
│ PIN 25 (IN4)    ─────┼────────────┤ IN4          │
│                      │            │              │
│ +5V ───────────┬─────┼────────────┤ +5V          │
│ GND ───────────┼─────┼────────────┤ GND          │
└──────────────────────┘            └──────────────┘
                 │
                 └────┬─────────────────────────┬──────────────────┐
                      │                         │                  │
                    +5V                        GND              I2C Bus
                                                              (SDA-20, SCL-21)
                                                                    │
                                                        ┌───────────┴─────────┐
                                                        │ MMA7660             │
                                                        │ Accelerometer       │
                                                        │ SDA: 20, SCL: 21    │
                                                        │ +5V, GND            │
                                                        └─────────────────────┘

Motor Bağlantıları:
Sol Motor:   L298N OUT1 & OUT2 ─── DC Motor
Sağ Motor:   L298N OUT3 & OUT4 ─── DC Motor
```

## ⚡ Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Arduino Board | Mega 2560 |
| Motor Sürücü | L298N |
| Motor Voltajı | 5-12V |
| PWM Frekansı | 490-980 Hz |
| HC-SR04 Voltajı | 5V |
| MMA7660 Protokol | I2C (400 kHz) |
| Temel Hız | 110 |
| Dönüş Hızı | 130 |
| Engel Mesafesi | 35 cm |

## 🔧 Bağlantı Adımları

1. **HC-SR04 Sensörleri Bağlayın:**
   - Ön: Trig→26, Echo→27
   - Sol: Trig→28, Echo→29
   - Sağ: Trig→30, Echo→31
   - Tümü: VCC→+5V, GND→GND

2. **L298N Motor Sürücüsünü Bağlayın:**
   - EN_A→PWM 2, IN1→23, IN2→22
   - EN_B→PWM 3, IN3→24, IN4→25
   - +5V→Arduino +5V, GND→Arduino GND

3. **DC Motorları Bağlayın:**
   - Sol Motor: L298N OUT1 & OUT2
   - Sağ Motor: L298N OUT3 & OUT4

4. **MMA7660 Accelerometer Bağlayın:**
   - SDA→20, SCL→21
   - VCC→+5V, GND→GND

5. **Beslenmeyi Kontrol Edin:**
   - Arduino: USB veya +5V
   - Motorlar: Harici 5-12V kaynak

## ⚠️ Önemli Notlar

- Motor sürücü (L298N) yeterli akım çekmek için harici beslenme kaynağı gerekebilir
- HC-SR04 sensörleri 15-45 cm aralığında en iyi çalışır
- MMA7660 I2C protokolü kullanır, başka I2C cihazları da kullanılabilir
- Yan sensörlerdeki hataları minimize etmek için kalibrasyon yapılır

## 🎯 Beklenen Sonuç

Robot engelleri algılayarak onlardan kaçınacak ve otonom olarak ortamda hareket edecektir.
