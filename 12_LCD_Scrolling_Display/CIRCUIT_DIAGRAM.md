# 📺 LCD Hareketli Yazı Gösterimi - Bağlantı Şeması

## 📌 Genel Bilgi
Bu proje 16x2 LCD ekranda Türkçe metni kaydırarak gösterir. LiquidCrystal kütüphanesi kullanılarak paralel haberleşme ile Arduino'ya bağlanır. Basit hareket (scrolling) efekti oluşturulur.

## 📋 Pin Bağlantıları

### LCD to Arduino

| LCD Pin | Açıklama | Arduino Pin |
|---------|---------|-----------|
| RS | Register Select | 12 |
| EN | Enable | 11 |
| D4 | Data Bit 4 | 5 |
| D5 | Data Bit 5 | 4 |
| D6 | Data Bit 6 | 3 |
| D7 | Data Bit 7 | 2 |
| VSS | Toprak | GND |
| VDD | +5V | +5V |
| VO | Kontrast | POT (GND - +5V) |
| RW | Okuma/Yazma | GND (yazma modu) |

## 🔌 Fiziksel Bağlantı Diyagramı

```
Arduino Uno/Nano
┌──────────────────────────┐
│                          │
│ 12 (RS) ─────────────────┼──── LCD RS
│ 11 (EN) ─────────────────┼──── LCD EN
│ 5 (D4) ──────────────────┼──── LCD D4
│ 4 (D5) ──────────────────┼──── LCD D5
│ 3 (D6) ──────────────────┼──── LCD D6
│ 2 (D7) ──────────────────┼──── LCD D7
│                          │
│ +5V ────────┬────────────┼──── LCD VDD
│             │            │
│ GND ─────┬──┼────────────┼──── LCD VSS/RW
└──────────┼──┼────────────┘
           │  │
           │  └─── LCD EN-2
           │
        [POT] ← Kontrast Kontrolü
        10kΩ  (isteğe bağlı)
           │
          GND

Detaylı LCD Bağlantısı:

LCD 1602 (16 karakter x 2 satır)
┌─────────────────────────────────┐
│ 16x2 LCD                        │
│ ┌─────────────────────────────┐ │
│ │ Satır 1: XXXXXXXXXXXXXXXX   │ │
│ │ Satır 2: XXXXXXXXXXXXXXXX   │ │
│ └─────────────────────────────┘ │
│                                 │
│ [1]  [2]  [3]  [4]  [5]  [6]    │
│ VSS  VDD  VO   RS   RW   EN     │
│ GND  +5V  POT  12   GND  11     │
│                                 │
│ [7]  [8]  [9] [10] [11] [12]    │
│ D0   D1   D2   D3   D4   D5     │
│ NC   NC   NC   NC    5    4     │
│                                 │
│ [13] [14] [15] [16]             │
│ D6   D7   LED+ LED-             │
│  3    2   +5V  GND              │
└─────────────────────────────────┘
```

## ⚡ Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| LCD Tipi | 16x2 (16 karakter, 2 satır) |
| Haberleşme | 4-bit Paralel (LCD'nin D4-D7) |
| Çalışma Voltajı | 5V |
| Veri Pinleri | 4 (D4, D5, D6, D7) |
| Kontrol Pinleri | 2 (RS, EN) |
| Max Karakter | 32 (16x2) |
| Metin | Türkçe UTF-8 |
| Scroll Delay | 200ms |

## 🔧 LCD Haberleşme Mantığı

### 4-bit Mod
- D0, D1, D2, D3 kullanılmıyor
- D4, D5, D6, D7 veri gönderimi için kullanılıyor
- Byte iki pulse'te gönderilir (yüksek nibble, düşük nibble)

### RS Pini (Register Select)
- RS = 0: Komut modu
- RS = 1: Veri modu

### EN Pini (Enable)
- EN = 1: Veri hazır
- EN = 0: Veri kaydedildi

## 🔧 Bağlantı Adımları

1. **LCD Pinlerini Arduino'ya Bağlayın:**
   - RS → PIN 12
   - EN → PIN 11
   - D4 → PIN 5
   - D5 → PIN 4
   - D6 → PIN 3
   - D7 → PIN 2
   - VSS (GND) → GND
   - VDD (+5V) → +5V
   - RW → GND (yazma modu)

2. **Kontrast Ayarlaması (Opsiyonel):**
   - 10kΩ potansiyometre:
     - Bir ucu → +5V
     - Ortası → LCD VO (PIN 3)
     - Diğer ucu → GND

3. **LED Arka Aydınlatma (Opsiyonel):**
   - LED+ → +5V (220Ω resistör üzerinden)
   - LED- → GND

4. **Kütüphane Yükleyin:**
   - Arduino IDE: Sketch → Include Library → Manage Libraries
   - "LiquidCrystal" arayın (Arduino tarafından resmi)
   - Yükleyin

5. **Kodu Yükleyin**

## ⚠️ Önemli Notlar

- 4-bit mod kullanılıyor (6 pin yetersiz ise 8-bit mod kullanılabilir)
- Kontrast VO pini ayarlanmazsa karakter görülmeyebilir
- LCD bacaklarındaki kapasitörler gerekli olabilir
- I2C LCD modülü daha az pin kullanır (isteğe bağlı)
- Metin UTF-8 olmalı (Turkish karakterler)

## 🔧 Detaylı Kod Açıklaması

```cpp
#include <LiquidCrystal.h>

// LCD pinleri: RS, EN, D4, D5, D6, D7
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char metin[] = "belki ustumuzden bir kus gecer, kanadindan bir tuy duser.  ";
char metins[17];  // 16 karakter + null terminator

void setup() {
  lcd.begin(16, 2);  // 16x2 LCD'yi başlat
}

void loop() {
  int uzunluk = strlen(metin);
  
  // Metni kaydır
  for (int j = 0; j < uzunluk; j++) {
    for (int i = 0; i < 16; i++) {
      // Modulo ile döngü yap
      metins[i] = metin[(i + j) % uzunluk];
    }
    metins[16] = '\0';  // String sonlandırıcı
    
    printslowly(metins);
  }
}

void printslowly(char* yaz) {
  lcd.clear();
  lcd.print(yaz);
  delay(200);  // 200ms hızında kaydır
}
```

## 🎯 Beklenen Sonuç

LCD ekranda şu şekilde görülür:

```
Başlangıç:
┌────────────────┐
│belki ustumuzd  │
│en bir kus gece │
└────────────────┘

Hareketli Gösterim:
┌────────────────┐
│elki ustumuzden │
│ bir kus gecer, │
└────────────────┘

┌────────────────┐
│lki ustumuzden  │
│bir kus gecer, │
└────────────────┘

...ve devamı tekrarlanır
```

## 📊 Timing Diyagramı

```
RS (Register):
   |‾‾‾‾|___|‾‾‾‾|___|  (veri/komut değeri)

EN (Enable):
   |  |_|   |_|   |_|    (veri yakalanması anları)

Metin Kaydırma Temposu:
Yazı her 200ms'de bir karakter sağa kaydırılır
```

## 💡 Öğrenme Noktaları

1. **Paralel Haberleşme**: Paralel veri gönderimi
2. **4-bit Mod**: Pin tasarrufu ile haberleşme
3. **LiquidCrystal Kütüphanesi**: LCD kontrol
4. **String İşleme**: Metin manipülasyonu
5. **Modulo Operatörü**: Döngü yapma

Bu proje LCD kullanımının temel örneğidir. Daha karmaşık uygulamalarda sıcaklık ölçümü, sensör verileri vb. gösterilebilir.

## 🔄 I2C LCD Alternatifu

Daha az pin kullanmak için I2C LCD modülü kullanılabilir:
- Sadece 4 pin gerekli: GND, +5V, SDA, SCL
- Adres: 0x27 (varsayılan)
- Kütüphane: LiquidCrystal_I2C
