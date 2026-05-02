# 📡 Ultrasonik Sensör Testi - Bağlantı Şeması

## 📌 Genel Bilgi
Bu proje HC-SR04 ultrasonik sensörü test etmek amacıyla yazılmış basit bir koddur. Sensör pulse gönderme mantığı doğru ancak ölçüm henüz programlanmamıştır. Bu dosya deneysel/tamamlanmamış bir projedir.

## 📋 Pin Bağlantıları

| Bileşen | Arduino Pin | Başka Pin | Açıklama |
|---------|------------|----------|---------|
| HC-SR04 Trig | 6 | - | Trigger sinyali (çıkış) |
| HC-SR04 Echo | 5 | - | Echo sinyali (giriş) |
| HC-SR04 VCC | +5V | - | Beslenme |
| HC-SR04 GND | GND | - | Toprak |

## 🔌 Fiziksel Bağlantı Diyagramı

```
Arduino Uno/Nano
┌──────────────────────────┐
│                          │
│ 6 (Trig) ────────────────┼──── HC-SR04 │Trig                      │
│                          │
│ 5 (Echo) ────────────────┼──── HC-SR04 │Echo                      │
│                          │
│ +5V ──────────────┬──────┼──── HC-SR04 VCC
│                   │      │
│ GND ──────────────┼──────┼──── HC-SR04 GND
└───────────────────┼──────┘
                    │
            HC-SR04 Sensör
            ┌──────────┐
            │          │
            │ Trig ◄── PIN 6
            │ Echo ◄── PIN 5
            │ VCC  ◄── +5V
            │ GND  ◄── GND
            │          │
            │   Ses    │
            │ Algılama │
            │          │
            └──────────┘
```

## ⚡ Teknik Özellikler

| Parametre | Değer |
|-----------|-------|
| Sensör | HC-SR04 Ultrasonik |
| Arduino Board | Arduino Uno/Nano/Mega |
| Trig Pin | 6 (Digital OUTPUT) |
| Echo Pin | 5 (Digital INPUT) |
| Çalışma Voltajı | 5V |
| Ölçüm Aralığı | 2cm - 400cm (teorik) |
| Pratik Aralık | 5cm - 100cm |
| Frekans | 40 kHz (ultrasonik) |
| Trigger Pulsu | 10µs |
| Echo Pulsu Süresi | 150µs - 25ms |

## 🔧 HC-SR04 Çalışma Prensibi

### Normal İşleyiş
1. **Trigger Pulse Gönder:**
   - Trig pini LOW tutun (en az 2µs)
   - Trig pini HIGH tutun (10µs)
   - Trig pini tekrar LOW tutun

2. **Sesli Dalga Yayınla:**
   - 8 tane 40 kHz pulse yayınlanır
   - Ses dalgası hedefe gider

3. **Echo Bekle:**
   - Echo pini timeout sonuna kadar HIGH tutar
   - Puls süresi = (mesafe × 2) / hız

4. **Mesafeyi Hesapla:**
   ```
   mesafe (cm) = (puls_süresi (µs) × 0.034) / 2
   ```

## 🔧 Güncel Kod Analizi

```cpp
#define trig 6
#define echo 5

void setup() {
  pinMode(trig, OUTPUT);
  pinmode(echo, INPUT);  // ⚠️ BUG: pinmode yerine pinMode olmalı
}

void loop() {
  digitalWrite(trig, LOW);
  delay(1);
  digitalWrite(trig, HIGH);
  delay(1);
  digitalWrite(trig, LOW);
  delay(1);
}
```

**Sorunlar:**
- Trigger pulse'ü düzgün şekilde oluşturmakta
- Echo pulsunu okumuyor
- Mesafe hesaplaması yapmıyor
- `pinmode()` fonksiyonu yanlış yazılan (`pinMode()` olmalı)

## 🔧 Bağlantı Adımları

1. **Sensörü Arduino'ya Bağlayın:**
   - HC-SR04 Trig → Arduino PIN 6
   - HC-SR04 Echo → Arduino PIN 5
   - HC-SR04 VCC → Arduino +5V
   - HC-SR04 GND → Arduino GND

2. **Wiring'i Kontrol Edin:**
   - Tüm bağlantılar sağlam mı?
   - Ters polarite yok mu?

3. **Kodu Yükleyin**

## ⚠️ Önemli Notlar

- Bu kod **tamamlanmamış** durumdadır
- Trigger pulse'ü gönderiliyor ancak echo okunmuyor
- Mesafe ölçümü hesaplanmıyor
- `pinmode()` yazım hatası düzeltilmelidir
- Seri haberleşme açılmamıştır (debug çıkışı yok)

## 🔧 Tamamlama İçin Gerekli Adımlar

```cpp
// 1. Seri haberleşme başlatın
Serial.begin(9600);

// 2. Echo time ölçümü yapın
unsigned long pulse_duration = pulseIn(echo, HIGH);

// 3. Mesafeyi hesaplayın
float distance_cm = pulse_duration * 0.034 / 2;

// 4. Sonuçları yazdırın
Serial.println(distance_cm);
```

## 🎯 Geliştirilmiş Kod Örneği

```cpp
#define TRIG 6
#define ECHO 5

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  // Trigger pulse gönder
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  // Echo time ölç
  unsigned long duration = pulseIn(ECHO, HIGH);
  
  // Mesafeyi hesapla
  float distance = duration * 0.034 / 2;
  
  // Sonucu yazdır
  Serial.print("Mesafe: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  delay(500);
}
```

## 📊 Ölçüm Tablosu

| Mesafe (cm) | Echo Süresi (µs) | Hesaplanan Mesafe |
|-------------|-----------------|------------------|
| 5 | 295 | 5.0 |
| 10 | 590 | 10.0 |
| 20 | 1180 | 20.0 |
| 50 | 2950 | 50.0 |
| 100 | 5900 | 100.0 |

## 💡 Öğrenme Noktaları

1. **digitalWrite()**: Digital çıkış kontrol
2. **digitalWrite()**: Digital giriş okuma
3. **pulseIn()**: Pulse süresi ölçümü
4. **Zamanla Ölçüm**: µs bazında hassas zamanlama
5. **Ultrasonik Prensibi**: Ses hızı ile mesafe hesapı

---

**Not:** Bu proje tamamlanması önerilen bir deneysel projedir. Ölçüm fonksiyonalitesini ekleyerek engel algılama robotlarında kullanılabilir.
