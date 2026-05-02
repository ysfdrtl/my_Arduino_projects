# 💡 Rastgele LED Seçici - Bağlantı Şeması

💻 **Kod Dosyası:** [RandomLEDSelector.ino](RandomLEDSelector.ino)

## 📌 Genel Bilgi
Bu proje 5 LED'i ve 1 push-button'ı kullanmaktadır. Butona her basıldığında 5 LED arasından rastgele biri seçilerek yanar, diğerleri söndürülür. `random()` fonksiyonu örneğidir.

## 📋 Pin Bağlantıları

| Bileşen | Arduino Pin | Açıklama |
|---------|------------|---------|
| LED 1 | 2 | Birinci LED |
| LED 2 | 3 | İkinci LED |
| LED 3 | 4 | Üçüncü LED |
| LED 4 | 5 | Dördüncü LED |
| LED 5 | 6 | Beşinci LED |
| Button | 8 | Buton input |
| GND | GND | Toprak (tüm komponentler) |
| +5V | +5V | Beslenme |

## 🔌 Fiziksel Bağlantı Diyagramı

```
Arduino Uno/Nano
┌──────────────────────────────────┐
│                                  │
│ 2 ───────┬─ LED 1 (Anode)        │
│ 3 ───────┼─ LED 2 (Anode)        │
│ 4 ───────┼─ LED 3 (Anode)        │
│ 5 ───────┼─ LED 4 (Anode)        │
│ 6 ───────┼─ LED 5 (Anode)        │
│          │                       │
│ 8 ───────┼─ Button               │
│          │                       │
│ A0 ──┐   │                       │
│      └──┬┼─ Random Seed (Gürültü)│
│        │ │                       │
│ +5V ───┴─┼─ LED Katodlar (+)    │
│ GND ─────┼─ Direnç & Button GND │
└──────────┼──────────────────────┘
           │
     [Resistors]
     220Ω x5
     (LED'ler için)
           │
        [Leds]
        (Katod)
           │
          GND
```

## ⚡ Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Arduino Board | Arduino Uno/Nano/Mega |
| LED Sayısı | 5 |
| LED Voltajı | 5V |
| LED Max Akımı | 20mA (Arduino sınırı) |
| Direnç Değeri | 220Ω x5 |
| Button Tipi | Push-button (momentary) |
| Random Range | 0-4 (5 seçenek) |
| Seed Kaynağı | Analog pin (A0) |

## 🔧 İşleyiş Mantığı

### Setup (Başlangıç)
```
1. PIN 2-6 OUTPUT olarak ayarla (LED pinleri)
2. PIN 8 INPUT olarak ayarla (Button)
3. A0 gürültü oku
4. randomSeed(analogRead(A0)) ile random seed ayarla
5. tmp = 2 (başlangıç LED pini)
```

### Loop (Döngü)
```
1. Rastgele sayı üret: 0-299
2. Sayıyı modulo 5'e böl: kalan 0-4
3. PIN'i ayarla: kalan + 2 = 2-6
4. Buton basıldı mı kontrol et
5. Eğer basıldı:
   - Eski LED'i söndür (tmp)
   - Yeni rastgele LED'i yak (rnd)
   - tmp = rnd olarak güncelle
   - Buton çıkılana kadar bekle
6. Başa dön
```

### Rastgele Seçim Formülü
```
random_value = random(300)          // 0-299
led_index = random_value % 5        // Kalan 0-4
pin = led_index + 2                 // PIN 2-6
```

## 🔧 Detaylı Bağlantı Diyagramı

```
PIN 2 ──┬──[220Ω]──┬─ LED (+)
        │          │
        └──────────┴─ LED (-)
                   │
                  GND

(PIN 3, 4, 5, 6 için aynı şekilde)

Button:
PIN 8 ──┬─ Button ─┬─ GND
        │          │
        └──────────┘

A0 (Random Seed):
PIN A0 ──┬─ Kütüphaneler/Gürültü
         │
        GND
```

## 🔧 Bağlantı Adımları

1. **LED'leri Bağlayın:**
   - LED 1-5 Anode (+) → Arduino PIN 2-6 (her biri 220Ω resistör üzerinden)
   - LED 1-5 Katod (-) → GND

2. **Button'ı Bağlayın:**
   - Button bir ayağı → Arduino PIN 8
   - Button diğer ayağı → GND

3. **Random Seed (Opsiyonel):**
   - Analog pin A0 açık bırakabilir (kütüphaneler gürültü yakalar)
   - Veya A0 → GND (referans için)

4. **Beslenmeyi Kontrol Edin:**
   - Arduino: USB
   - 5V ve GND doğru bağlanmış olmalı

5. **Kodu Yükleyin**

## ⚠️ Önemli Notlar

- randomSeed() çeşitli sonuçlar için A0 gürültüsünü kullanır
- Eğer random sonuçlar tekrarlanıyorsa seed'i değiştirebilirsiniz
- Her LED için ayrı 220Ω resistör gerekli (5 adet)
- LED polaritesini doğru bağlayın (+ uzun bacak, - kısa bacak)
- Button basılı tutulması sırasında LED değişmez
- Buton bırakıldıktan sonra tekrar basılırsa yeni LED seçilir

## 🎯 Beklenen Sonuç

- Program başladığında:
  1. PIN 2'deki LED başlar
  2. Butona basıldığında rastgele bir LED (2-6 arası) seçilir ve yanır
  3. Diğer 4 LED söndürülür
  4. Tekrar butona basılırsa yeni LED seçilir (aynı olabilir)
  5. İşlem tekrar eder

## 📊 Örnek Seçim Sırası

```
Buton Basılışı | random() | % 5 | PIN | LED Seçimi
1              | 145      | 0   | 2   | LED 1 (yanıyor)
2              | 278      | 3   | 5   | LED 4 (yanıyor)
3              | 42       | 2   | 4   | LED 3 (yanıyor)
4              | 199      | 4   | 6   | LED 5 (yanıyor)
5              | 88       | 3   | 5   | LED 4 (yanıyor)
```

## 💡 Öğrenme Noktaları

1. **random() Fonksiyonu**: Rastgele sayı üretme
2. **Modulo Operatörü (%)**: Belirli aralıkta sayı elde etme
3. **randomSeed()**: Seed ayarlama
4. **Loop Array Logic**: Birden çok cihazın kontrol edilmesi
5. **Button Debouncing**: Buton durumunun stabili zasyonu

Bu proje başlangıç seviyesine uygun interaktif bir projedir.
