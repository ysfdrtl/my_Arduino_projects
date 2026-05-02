# 📊 İvmeölçer Hareketi İzleme - Bağlantı Şeması

## 📌 Genel Bilgi
Bu proje MMA7660 3 eksenli ivmeölçer sensörü (accelerometer) kullanarak hareketi izlemektedir. I2C protokolü ile Arduino'ya bağlanır ve hızlandırma, hız, konum verilerini hesaplar.

## 📋 Pin Bağlantıları

### MMA7660 Accelerometer (I2C Protokolü)

| Bileşen | Arduino Pin | Açıklama |
|---------|------------|---------|
| SDA (Veri) | 20 (SDA) veya 4 | I2C Veri hattı |
| SCL (Saat) | 21 (SCL) veya 5 | I2C Saat hattı |
| VCC | +5V | Beslenme |
| GND | GND | Toprak |
| INT (İnterapt) | - | Kullanılmıyor |

### Seri Haberleşme (İsteğe Bağlı)

| Bileşen | Arduino Pin | Açıklama |
|---------|------------|---------|
| TX | - | Debug çıkışı (Baud: 115200) |

## 🔌 Fiziksel Bağlantı Diyagramı

```
Arduino Mega 2560
┌──────────────────────┐
│                      │
│ I2C Bus:             │
│                      │
│ 20 (SDA) ────┬───────┼─────── SDA
│              │       │        (Veri hattı)
│ 21 (SCL) ────┼───────┼─────── SCL
│              │       │        (Saat hattı)
│              │       │
│              │   ┌───┴─────────────┐
│              │   │ MMA7660 Sensor  │
│              │   │                 │
│              └──→│ SDA             │
│              ┌──→│ SCL             │
│              │   │                 │
│              │   │ VCC: +5V        │
│              │   │ GND: GND        │
│ +5V ──────┬──┼──→│ +5V             │
│ GND ──────┼──┼──→│ GND             │
└───────────┼──┼──→└─────────────────┘
            │  │
            │  └──── 4.7kΩ Pull-up Dirençleri
            │        (SDA ve SCL için)
            │
            └─── +5V

I2C Bus Yapısı:
┌─────────────────┬─────────────────┐
│  Arduino Mega   │  MMA7660        │
│                 │                 │
│  SDA (20) ◄────┬─┤ SDA            │
│  SCL (21) ◄────┼─┤ SCL            │
│  +5V ────────┬─┼─┤ VCC            │
│  GND ────────┼─┼─┤ GND            │
│              │ │ │                │
│              └─┴─┤ 4.7kΩ Pull-up  │
│                 │ (SDA & SCL)     │
│                 │                 │
│                 └─────────────────┘
```

## ⚡ Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Sensor | MMA7660 3-eksen accelerometer |
| Arduino Board | Mega 2560 |
| Haberleşme | I2C (400 kHz standart mod) |
| I2C Adresi | 0x4C (varsayılan) |
| İvme Aralığı | ±1.5g, ±2g, ±4g, ±8g |
| Çözünürlük | 8-bit |
| Beslenme Voltajı | 3.3V - 3.6V veya 5V (kütüphaneye bağlı) |
| Seri Baud | 115200 bps (debug çıkışı) |
| Filtre Katsayısı (Alpha) | 0.85 |
| Kalibrasyon Örnekleri | 200 |

## 🔧 İvmeölçer Kalibrasyon Mantığı

1. **Başlangıç Kalibrasyon:**
   - Sensör sabit tutulur
   - 200 örnek alınır
   - Ortalama ofset hesaplanır

2. **Filtre Uygulanması:**
   - Gürültü azaltmak için exponential filter kullanılır
   - Alpha = 0.85 (düşük değer = daha fazla filtre)

3. **Ölçüm Yapılması:**
   - İvme (acceleration)
   - Hız (velocity) - integral
   - Konum (position) - integral

## 🔧 Bağlantı Adımları

1. **I2C Pinlerini Bağlayın:**
   - MMA7660 SDA → Arduino 20 (veya 4 - doğru portu kontrol edin)
   - MMA7660 SCL → Arduino 21 (veya 5 - doğru portu kontrol edin)

2. **Beslenmeyi Bağlayın:**
   - MMA7660 VCC → Arduino +5V
   - MMA7660 GND → Arduino GND

3. **Pull-up Dirençleri (Opsiyonel):**
   - 4.7kΩ direnç: +5V → SDA
   - 4.7kΩ direnç: +5V → SCL
   - (Çoğu kütüphane internal pull-up aktifleştirir)

4. **Kütüphane Yükleyin:**
   - Arduino IDE: Sketch → Include Library → Manage Libraries
   - "MMA7660" arayın ve yükleyin

5. **Seri Monitörü Açın:**
   - Arduino IDE: Tools → Serial Monitor
   - Baud: 115200

## ⚠️ Önemli Notlar

- I2C haberleşmesi çift yönlüdür (open-drain)
- Pull-up dirençleri yüksek empedans için gerekebilir
- MMA7660 hassas bir sensördür, titreşimden uzak tutun
- Kalibrasyon ederken sensörü sabit tutun
- I2C adresi varsayılan olarak 0x4C'dir
- Diğer I2C cihazları aynı bus'a bağlanabilir

## 🎯 Beklenen Sonuç

Program başladığında:
1. "Sensoru sabit tut, kalibrasyon yapiliyor..." mesajı görülür
2. 2 saniye bekleme
3. 200 örnek alınarak offset hesaplanır
4. Sensör hareketlere karşılık ivme, hız ve konum değerlerini hesaplamaya başlar
5. Seri monitor'de veriler gösterilir

## 📊 Örnek Çıkış Verileri

```
Akselerasyon X: -0.15 g
Hız X: 0.05 m/s
Konum X: 0.01 m

Filtreli İvme: -0.12 g
```

Bu değerler sensörün hareket algılamasını gösterir.
