# ⚙️ Motor Temel Test - Bağlantı Şeması

## 📌 Genel Bilgi
Bu proje 2 DC motoru L298N motor sürücüsü aracılığıyla kontrol etmektedir. Motor hızı, yönü ve temel hareket testleri yapılır.

## 📋 Pin Bağlantıları

### Arduino to L298N

| Arduino Pin | L298N Pin | Fonksiyon | Motor |
|------------|----------|----------|-------|
| PWM 5 | EN_A | Motor Hız Kontrol | Sol |
| 8 | IN1 | Yön Kontrol | Sol |
| 9 | IN2 | Yön Kontrol | Sol |
| PWM 6 | EN_B | Motor Hız Kontrol | Sağ |
| 10 | IN3 | Yön Kontrol | Sağ |
| 11 | IN4 | Yön Kontrol | Sağ |
| +5V | +5V | Beslenme | - |
| GND | GND | Toprak | - |

### Motor Çıkışları

| Motor | L298N Çıkış | Bağlantı |
|-------|-----------|---------|
| Sol Motor | OUT1, OUT2 | DC Motor 1 |
| Sağ Motor | OUT3, OUT4 | DC Motor 2 |

## 🔌 Fiziksel Bağlantı Diyagramı

```
Arduino Uno/Nano
┌─────────────────────────┐
│                         │
│ PWM5 (EN_A)  ───────────┼───────── EN_A (L298N)
│ 8 (IN1)      ───────────┼───────── IN1
│ 9 (IN2)      ───────────┼───────── IN2
│                         │
│ PWM6 (EN_B)  ───────────┼───────── EN_B (L298N)
│ 10 (IN3)     ───────────┼───────── IN3
│ 11 (IN4)     ───────────┼───────── IN4
│                         │
│ +5V ──────────┬─────────┼───────── +5V (L298N)
│ GND ──────────┼─────────┼───────── GND (L298N)
│               │         │
└───────────────┼─────────┘
                │
        ┌───────┴──────────┐
        │    L298N         │
        │                  │
        │ EN_A: PWM5 ◄──   │
        │ IN1: 8 ◄──       │
        │ IN2: 9 ◄──       │
        │                  │
        │ EN_B: PWM6 ◄──   │
        │ IN3: 10 ◄──      │
        │ IN4: 11 ◄──      │
        │                  │
        │ OUT1 ──────┐     │
        │ OUT2 ──────┼──── Sol Motor (+/-)
        │            │     │
        │ OUT3 ──────┐     │
        │ OUT4 ──────┼──── Sağ Motor (+/-)
        │            │     │
        └────────────┴─────┘
```

## ⚡ Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Arduino Board | Arduino Uno/Nano |
| Motor Sürücü | L298N |
| Motor Voltajı | 5-12V |
| PWM Frekansı | 490-980 Hz |
| Motor Hızı | 180 (0-255) |
| Test Süresi | 500 ms (her hareket) |

## 🔧 Motor Kontrol Mantığı

### İleri Hareketi (ileriGit)
- Sol Motor: IN1=HIGH, IN2=LOW (İleri)
- Sağ Motor: IN3=HIGH, IN4=LOW (İleri)
- Hız: 180 (0-255)

### Durma (dur)
- Sol Motor: IN1=LOW, IN2=LOW (Durdur)
- Sağ Motor: IN3=LOW, IN4=LOW (Durdur)

### Sağa Dönme (saga90Don)
- Sol Motor: IN1=HIGH, IN2=LOW (İleri)
- Sağ Motor: IN1=HIGH, IN2=LOW (İleri - Araç kendi etrafında döner)

## 🔧 Bağlantı Adımları

1. **Arduino Pinlerini L298N'e Bağlayın:**
   - EN_A (hız kontrol): Arduino PWM 5
   - IN1, IN2 (yön): Arduino 8, 9
   - EN_B (hız kontrol): Arduino PWM 6
   - IN3, IN4 (yön): Arduino 10, 11

2. **DC Motorları Bağlayın:**
   - Sol Motor: L298N OUT1 & OUT2
   - Sağ Motor: L298N OUT3 & OUT4

3. **Beslenmeyi Bağlayın:**
   - L298N +5V: Arduino +5V
   - L298N GND: Arduino GND
   - Motor +12V kaynağı (harici - opsiyonel)

## ⚠️ Önemli Notlar

- PWM pinleri (5, 6) motor hızını kontrol eder
- Hız değeri 0-255 arasında olmalıdır
- Motor sürücü harici beslenme kaynağı gerektirebilir
- Geç yanıt verirse delay() değerlerini ayarlayın

## 🎯 Beklenen Sonuç

Program başladığında:
1. Motorlar 500ms ileri hareket eder
2. 500ms durma
3. 90 derece sağa döner
4. 500ms durma
5. Döngü tekrar eder
