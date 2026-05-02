# ⚙️ 4 Kanallı Motor Kontrol - Bağlantı Şeması

## 📌 Genel Bilgi
Bu proje 4 DC motoru (2 üst seviye, 2 alt seviye) L298N motor sürücü kullanarak kontrol etmektedir. Çok kanallı motor kontrolü ve test amaçlıdır.

## 📋 Pin Bağlantıları

### PWM Hız Kontrol Pinleri

| Motor | PWM Pin | Seviye | Fonksiyon |
|-------|---------|--------|----------|
| Motor A (Üst) | PWM 11 | Üst | Hız Kontrolü |
| Motor B (Üst) | PWM 10 | Üst | Hız Kontrolü |
| Motor A (Alt) | PWM 9 | Alt | Hız Kontrolü |
| Motor B (Alt) | PWM 8 | Alt | Hız Kontrolü |

### Yön Kontrol Pinleri

| Kontrol | PIN | Motor | Açıklama |
|---------|-----|-------|---------|
| A2U | 22 | A Üst | Yön 1 |
| A1U | 24 | A Üst | Yön 2 |
| B1U | 26 | B Üst | Yön 1 |
| B2U | 28 | B Üst | Yön 2 |
| A2A | 30 | A Alt | Yön 1 |
| A1A | 32 | A Alt | Yön 2 |
| B1A | 34 | B Alt | Yön 1 |
| B2A | 36 | B Alt | Yön 2 |

## 🔌 Fiziksel Bağlantı Diyagramı

```
Arduino Mega 2560
┌─────────────────────────────┐
│                             │
│ PWM 11 (pwmAU)  ────────────┼─────── Motor A Üst (Hız)
│ 22 (A2U) ───────────────────┼─────── Motor A Üst (Yön 1)
│ 24 (A1U) ───────────────────┼─────── Motor A Üst (Yön 2)
│                             │
│ PWM 10 (pwmBU)  ────────────┼─────── Motor B Üst (Hız)
│ 26 (B1U) ───────────────────┼─────── Motor B Üst (Yön 1)
│ 28 (B2U) ───────────────────┼─────── Motor B Üst (Yön 2)
│                             │
│ PWM 9 (pwmAA)   ────────────┼─────── Motor A Alt (Hız)
│ 30 (A2A) ───────────────────┼─────── Motor A Alt (Yön 1)
│ 32 (A1A) ───────────────────┼─────── Motor A Alt (Yön 2)
│                             │
│ PWM 8 (pwmBA)   ────────────┼─────── Motor B Alt (Hız)
│ 34 (B1A) ───────────────────┼─────── Motor B Alt (Yön 1)
│ 36 (B2A) ───────────────────┼─────── Motor B Alt (Yön 2)
│                             │
│ +5V ──────┬─────────────────┼─────── +5V
│ GND ──────┼─────────────────┼─────── GND
└───────────┼─────────────────┘
            │
    4x L298N Motor Sürücü
    (Paralel Bağlı veya Dış)
            │
    ┌───────┴─────────┬───────────┬──────────┐
    │                 │           │          │
Motor A Üst    Motor B Üst    Motor A Alt  Motor B Alt
   (+/-)          (+/-)          (+/-)       (+/-)
```

## ⚡ Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Arduino Board | Mega 2560 |
| Motor Sürücü | L298N (x4 veya parallele) |
| Motor Voltajı | 5-12V |
| PWM Frekansı | 490-980 Hz |
| Max PWM Değeri | 255 |
| Hız Seviyeleri | 0-255 |
| Toplam Motor | 4 |

## 🔧 Motor Kontrol Mantığı

### İleri Hareketi (ileri)
```
Motor A Üst:  A1U=HIGH, A2U=LOW, pwmAU=255
Motor B Üst:  B1U=HIGH, B2U=LOW, pwmBU=255
Motor A Alt:  A1A=HIGH, A2A=LOW, pwmAA=255
Motor B Alt:  B1A=HIGH, B2A=LOW, pwmBA=255
Yön: İleri
Süre: 1000ms
```

### Durma (dur)
```
Tüm motorlar: Yön=LOW, pwm=0
Süre: 500ms
```

### Geri Hareketi (geri)
```
Motor A Üst:  A2U=HIGH, A1U=LOW, pwmAU=255
Motor B Üst:  B2U=HIGH, B1U=LOW, pwmBU=255
Motor A Alt:  A2A=HIGH, A1A=LOW, pwmAA=255
Motor B Alt:  B2A=HIGH, B1A=LOW, pwmBA=255
Yön: Geri
Süre: 1000ms
```

## 🔧 Bağlantı Adımları

1. **PWM ve Yön Pinlerini Bağlayın:**
   - PWM pinleri (8-11) motor hızına
   - Yön pinleri (22-36 çift sayılar) yön kontrolüne

2. **L298N Motor Sürücülerini Bağlayın:**
   - Her motor için ayrı sürücü veya parallele bağlı sürücü
   - Tüm sürücüleri Arduino GND ve +5V'ye bağlayın

3. **DC Motorları Bağlayın:**
   - Motor A Üst: Sürücü çıkışlarından biri
   - Motor B Üst: Sürücü çıkışlarından biri
   - Motor A Alt: Sürücü çıkışlarından biri
   - Motor B Alt: Sürücü çıkışlarından biri

4. **Beslenmeyi Sağlayın:**
   - Arduino: USB
   - Motor Sürücüsü: +5V (Arduino veya harici)
   - Motorlar: 5-12V harici kaynak (önerilir)

## ⚠️ Önemli Notlar

- 4 motor kullanıldığı için yeterli akım kaynağı gereklidir
- PWM frekansı motorların performansını etkiler
- Yön pinleri mantık seviyesidir (HIGH/LOW)
- Hız değerleri 0-255 arasında olmalıdır

## 🎯 Beklenen Sonuç

Program başladığında:
1. Tüm motorlar 1 saniye ileri hareket eder
2. 500ms durma
3. Tüm motorlar 1 saniye geri hareket eder
4. 500ms durma
5. Döngü tekrar eder

Tüm motorlar eş zamanlı hareket eder (sinkronize).
