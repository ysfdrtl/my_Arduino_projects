# 💡 Buton Kontrollü LED Açılıp Kapanışı - Bağlantı Şeması

## 📌 Genel Bilgi
Bu proje basit bir push-button ve LED kullanmaktadır. Butona her basıldığında LED açılıp kapatılır (toggle). Giriş/çıkış operasyonlarının temel örneğidir.

## 📋 Pin Bağlantıları

| Bileşen | Arduino Pin | Başka Pin | Açıklama |
|---------|------------|----------|---------|
| LED (+) | 13 (onboard LED) | - | LED pozitif (built-in) |
| LED (-) | GND | - | LED negatif (toprak) |
| Button | 8 | - | Buton input pini |
| Button GND | GND | - | Buton toprak |

## 🔌 Fiziksel Bağlantı Diyagramı

```
Arduino Uno/Nano
┌──────────────────────┐
│                      │
│ 13 (LED) ────────────┼─────── LED (+)
│                      │
│ 8 (Button) ─────┬────┼─────── Button
│                 │    │
│ GND ────────┬───┼────┼─────── GND
│             │   │    │
└─────────────┼───┼────┘
              │   │
              │   └─────┬─────── 10kΩ Pull-up
              │         │       (isteğe bağlı)
              │         │
              ├─ Button ─┤
              │         │
              └─ +5V ───┘

Detaylı Şema:

Arduino PIN 13
     ↓
   [~] LED (Anode)
     │
   ─┴─ (~ = resistor)
     │
    ─┴─ Kathode
     │
    GND


Arduino PIN 8
     ↓
    [Button]
     ↓
    GND

(PIN 8 iç pull-up aktivleştirilmiştir)
```

## ⚡ Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Arduino Board | Arduino Uno/Nano/Mega |
| LED Tipi | Digital (2 seviyeli - ON/OFF) |
| LED Çalışma Voltajı | 5V |
| LED Max Akımı | 20mA (Arduino sınırı) |
| Button Tipi | Push-button (momentary) |
| Button Voltajı | 5V |
| Input Mode | INPUT_PULLUP |
| Bounce Süresi | 1ms debouncing |

## 🔧 İşleyiş Mantığı

### Setup (Başlangıç)
```
- PIN 13'ü OUTPUT olarak ayarla (LED çıkışı)
- PIN 8'i INPUT_PULLUP olarak ayarla (Button girişi)
- Pullup direnç iç olarak aktifleştirilir
```

### Loop (Döngü)
```
1. PIN 8 değerini oku
2. Eğer buton basılı (!digitalRead(8)):
   - LED durumunu tersine çevir (toggle)
   - Buton değeri şu an yazarsa while() ile bekle
3. Buton çıkılana kadar bekle
4. Başa dön
```

### LED Toggle
```
Yazan = 0 (LED kapalı)
↓
Buton basıldı
↓
Yazan = 1 (LED açık)
↓
Buton basıldı
↓
Yazan = 0 (LED kapalı)
```

## 🔧 Bağlantı Adımları

### Seçenek 1: Built-in LED Kullanma (En Kolay)
1. Buton → Arduino PIN 8
2. Buton diğer ayağı → GND
3. Kodu yükleyin
4. Buton PIN 13 built-in LED'i kontrol eder

### Seçenek 2: Harici LED Kullanma
1. LED anod (+) → 220Ω resistör → Arduino PIN 13
2. LED katod (-) → GND
3. Buton → Arduino PIN 8
4. Buton diğer ayağı → GND
5. Kodu yükleyin

### Seçenek 3: Pull-up Direnci Ekleme (İsteğe Bağlı)
1. 10kΩ resistör: +5V → Arduino PIN 8
2. Buton: PIN 8 → GND
3. LED ve PIN 13 bağlantısı aynı

## ⚠️ Önemli Notlar

- Arduino PIN 13'te built-in LED bulunur (Uno/Nano)
- INPUT_PULLUP iç pull-up direncini aktifleştirir
- Butona basıldığında PIN 8 LOW (0) olur
- Buton çıkıldığında PIN 8 HIGH (1) olur
- Debouncing (1ms delay) button bounce'unu önler
- LED'i eğer çok parlak görüyorsanız direnç değerini artırın (330Ω, 470Ω)

## 🎯 Beklened Sonuç

- Program başladığında LED kapalı
- Butona her basıldığında LED açılıp kapanır
- Buton basılı tutulması LED durumunu değiştirmez
- Yalnız buton bırakıldıktan sonra tekrar basılırsa LED durumu değişir



## 💡 Öğrenme Noktaları

1. **Digital INPUT**: Buton okuma
2. **Digital OUTPUT**: LED kontrolü
3. **INPUT_PULLUP**: İç pull-up resistör kullanımı
4. **Toggle Logic**: Durumun ters çevrilmesi
5. **Debouncing**: Buton tıklamasından etkilenme

Bu proje Arduino öğreniminin temel başlangıç projelerinden biridir.
