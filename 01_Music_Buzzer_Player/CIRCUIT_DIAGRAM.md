# 🎵 Müzik/Buzzer Müzisyeni - Bağlantı Şeması

💻 **Kod Dosyası:** [MusicBuzzer_Manifesto.ino](MusicBuzzer_Manifesto.ino)

## 📌 Genel Bilgi
Bu proje pasif bir buzzer kullanarak Arduino üzerinde müzik çalmaktadır. Buzzer, PIN 48 üzerinden kontrol edilir.

## 📋 Pin Bağlantıları

| Bileşen | Arduino Pin | Başka Pin | Açıklama |
|---------|------------|----------|---------|
| Buzzer (+) | PIN 48 | - | Buzzer pozitif bağlantısı |
| Buzzer (-) | GND | - | Buzzer negatif (toprak) |

## 🔌 Fiziksel Bağlantı Diyagramı

```
Arduino Mega 2560
┌─────────────────────┐
│                     │
│  48 ──────┬─────────── Buzzer (+)
│           │
│  GND ─────┴─────────── Buzzer (-)
│                     │
└─────────────────────┘
```

## ⚡ Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Çalışma Voltajı | 5V |
| Buzzer Tipi | Pasif |
| Frekans Aralığı | 262-659 Hz |
| Tempo | 132 BPM |
| PWM Kullanımı | Evet (tone()) |

## 🔧 Bağlantı Adımları

1. Buzzerin pozitif bacağını Arduino PIN 48'e bağlayın
2. Buzzerin negatif bacağını Arduino GND'ye bağlayın
3. Kodu Arduino IDE'ye yükleyin
4. Müzik çalmaya başlaması için beklemeniz yeterlidir

## ⚠️ Önemli Notlar

- Pasif buzzer PWM sinyali (tone()) gerektirir
- Aktif buzzer kullanıyorsanız doğrudan 5V'ye bağlayabilirsiniz (PIN 48 gerekli değil)
- Ses seviyesi çok yüksek olabilir, dikkatli olun
- Beslenme kaynağı yeterli akım sağlamalıdır

## 🎯 Beklenen Sonuç

Program başladığında "Manifesto" şarkısı buzzer üzerinden çalınacaktır.
