# ⚙️ 2 Kanallı Motor Kontrol Testi - Bağlantı Şeması

💻 **Kod Dosyası:** [Motor2ChannelBasic.ino](Motor2ChannelBasic.ino)

## 📌 Genel Bilgi
Bu proje 2 DC motoru temel seviyede kontrol etmektedir. Motor hızı ve yönü basit olarak kontrol edilir.

## 📋 Pin Bağlantıları

### PWM Hız Kontrol Pinleri

| Motor | PWM Pin | Fonksiyon |
|-------|---------|----------|
| Motor A | PWM 2 | Hız Kontrolü |
| Motor B | PWM 3 | Hız Kontrolü |

### Yön Kontrol Pinleri

| Kontrol | PIN | Motor | Açıklama |
|---------|-----|-------|---------|
| A1 | 22 | A | Yön 1 |
| A2 | 23 | A | Yön 2 |
| B1 | 24 | B | Yön 1 |
| B2 | 25 | B | Yön 2 |

## 🔌 Fiziksel Bağlantı Diyagramı

```
Arduino Uno/Nano
┌───────────────────────┐
│                       │
│ PWM 2 (pwma)  ────────┼──────── Motor A Hız
│ 22 (a1)       ────────┼──────── Motor A Yön 1
│ 23 (a2)       ────────┼──────── Motor A Yön 2
│                       │
│ PWM 3 (pwmb)  ────────┼──────── Motor B Hız
│ 24 (b1)       ────────┼──────── Motor B Yön 1
│ 25 (b2)       ────────┼──────── Motor B Yön 2
│                       │
│ +5V ──────────┬───────┼──────── +5V
│ GND ──────────┼───────┼──────── GND
└───────────────┼───────┘
                │
        ┌───────┴──────────┐
        │  L298N Driver    │
        │                  │
        │ pwma: PWM2       │
        │ a1: 22, a2: 23   │
        │ pwmb: PWM3       │
        │ b1: 24, b2: 25   │
        │                  │
        │ OUT1,OUT2 ─────┐ │
        │  (Motor A)     │ │
        │                │ │
        │ OUT3,OUT4 ─────┤ │
        │  (Motor B)     │ │
        │                │ │
        └────────────────┼─┘
                         │
         ┌───────────────┴──────────────┐
         │                              │
       Motor A                       Motor B
       (+/-)                         (+/-)
```

## ⚡ Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Arduino Board | Arduino Uno/Nano/Mega |
| Motor Sürücü | L298N |
| Motor Voltajı | 5-12V |
| PWM Frekansı | 490-980 Hz (Uno/Nano) |
| PWM Değeri | 170 (0-255) |
| Test Süresi | 2 saniye ileri + 2 saniye durma |

## 🔧 Motor Kontrol Mantığı

### Motor Hareketi

1. **İleri Hareketi:**
   ```
   Motor A: a1=HIGH, a2=LOW, pwm=170
   Motor B: b1=HIGH, b2=LOW, pwm=170
   Hız: 170 (orta hız)
   Süre: 2000ms (2 saniye)
   ```

2. **Durma:**
   ```
   Motor A: a1=LOW, a2=LOW, pwm=0
   Motor B: b1=LOW, b2=LOW, pwm=0
   Süre: 2000ms (2 saniye)
   ```

3. **Döngü Tekrarı:**
   Program başladığında bu işlem tekrar eder

## 🔧 Bağlantı Adımları

1. **PWM Pinlerini Bağlayın:**
   - Motor A Hızı: Arduino PWM 2 → L298N EN_A
   - Motor B Hızı: Arduino PWM 3 → L298N EN_B

2. **Yön Kontrol Pinlerini Bağlayın:**
   - Motor A: 22→IN1, 23→IN2
   - Motor B: 24→IN3, 25→IN4

3. **DC Motorları Bağlayın:**
   - Motor A: L298N OUT1 & OUT2
   - Motor B: L298N OUT3 & OUT4

4. **Beslenmeyi Bağlayın:**
   - L298N +5V: Arduino +5V
   - L298N GND: Arduino GND
   - Motorlar için harici 5-12V kaynağı (opsiyonel)

## ⚠️ Önemli Notlar

- PWM değeri 170 (0-255 ölçeğinde orta hız)
- Yön pinleri digital seviyedir (HIGH/LOW)
- Motor sürücü yeterli akım sağlamak için harici beslenme gerekebilir
- Kod basit olduğu için eğitim ve test amaçlı idealdir

## 🎯 Beklenen Sonuç

Program başladığında:
1. Her 2 saniye ileri hareket eder
2. Her 2 saniye durma
3. Döngü tekrar eder

Her iki motor eş zamanlı hareket eder.
