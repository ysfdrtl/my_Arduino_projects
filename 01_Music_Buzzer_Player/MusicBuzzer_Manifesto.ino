/*
  PROJE: Müzik/Buzzer Müzisyeni
  AÇIKLAMA: Arduino üzerinde pasif buzzer kullanarak müzik çalma
  Yapıştırılan nota tablosu kullanarak 'Manifesto' şarkısını çalar
  KULLANILAN BILEŞENLER: Pasif buzzer
  NOTLAR: Buzzer PIN 48'e bağlıdır
*/

// KTS / Manifest
// Arduino passive buzzer code
// Based on the uploaded sheet music on page 1

#define BUZZER_PIN 48
#define REST 0

// Notes
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659

// Tempo
int tempo = 132;

// duration divider:
// 4 = quarter, 8 = eighth, 2 = half
int melody[] = {
  // Intro
  NOTE_F4, NOTE_E4, REST,
  NOTE_F4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_B4,
  NOTE_F4, NOTE_E4, REST,
  NOTE_F4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_B4,

  // Repeat-like phrase
  REST,
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_B4, NOTE_B4,
  NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_B4,

  REST,
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_B4, NOTE_B4,
  NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_B4,

  // "Ama yok inatla..."
  NOTE_A4, NOTE_B4,
  REST,
  NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4,
  NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_D5,
  NOTE_C5, NOTE_C5, NOTE_B4,
  REST,
  NOTE_D5, NOTE_C5, NOTE_C5,

  // "Hiç elimde yok..."
  REST,
  NOTE_E4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_E4,
  REST,
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_E4,

  REST,
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_E4,

  // "Bu kalp yerinde durmuyor..."
  REST,
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4,

  REST,
  NOTE_E4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_F4,

  REST,
  NOTE_E4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_F4,

  REST,
  NOTE_E4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_E4,

  // "Bonnie'm Clyde..."
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4,
  NOTE_G4, NOTE_G4,
  NOTE_F4, NOTE_F4,
  NOTE_E4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_F4,

  // "Son düştüm..."
  NOTE_A4, NOTE_A4, NOTE_F4, NOTE_F4, NOTE_E4,

  REST,
  NOTE_E4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_F4,

  // "Sor ma ne haldeyim..."
  NOTE_A4, NOTE_A4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_G4,
  REST,
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_D4, NOTE_F4,
  REST,
  NOTE_F4, NOTE_F4, NOTE_D4, NOTE_G4,

  // Ending-ish phrase
  NOTE_F4, NOTE_E4, NOTE_F4, NOTE_D4, NOTE_F4,
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_A4,
  NOTE_F4, NOTE_F4,

  NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_B4, NOTE_D4, NOTE_E4,
  REST,

  NOTE_D4, NOTE_E4, NOTE_B4,
  NOTE_D4, NOTE_E4, NOTE_B4,
  NOTE_D4, NOTE_E4,
  REST,

  NOTE_D4, NOTE_E4, NOTE_B4,
  NOTE_D4, NOTE_E4, NOTE_B4,
  NOTE_D4, NOTE_E4,
  REST,

  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_D4,
  NOTE_D4, NOTE_D4, NOTE_F4,
  NOTE_E4, NOTE_B4, NOTE_B4,

  NOTE_E4, NOTE_B4, NOTE_B4, NOTE_B4,
  NOTE_B4, NOTE_B4, NOTE_D4, NOTE_E4, NOTE_B4, NOTE_B4,

  NOTE_E4, NOTE_B4, NOTE_B4, NOTE_B4,
  NOTE_B4, NOTE_D4, NOTE_E4, NOTE_B4, NOTE_B4,

  // Final
  NOTE_E4, NOTE_E4, NOTE_D4,
  NOTE_D4, NOTE_C4, NOTE_C4,
  NOTE_B4, NOTE_B4, NOTE_C5,
  NOTE_B4, NOTE_B4, NOTE_B4,
  NOTE_A4, NOTE_B4, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_B4
};

int durations[] = {
  // Intro
  8, 8, 8,
  8, 8, 8, 8, 4,
  8, 8, 8,
  8, 8, 8, 8, 4,

  8,
  8, 8, 8, 8,
  8, 8,
  8, 8, 8, 8, 4,

  8,
  8, 8, 8, 8,
  8, 8,
  8, 8, 8, 8, 4,

  8, 8,
  8,
  8, 8, 8, 8, 4,
  8, 8, 8, 8, 4,
  8, 8, 4,
  8,
  8, 8, 4,

  8,
  8, 8, 8, 8, 8, 4,
  8,
  8, 8, 8, 8,
  8, 8, 8, 4,

  8,
  8, 8, 8, 8, 8, 4,

  8,
  8, 8, 8,
  8, 8, 8, 8, 4,

  8,
  8, 8, 8, 8, 8, 4,

  8,
  8, 8, 8, 8, 8, 4,

  8,
  8, 8, 8, 8, 4,

  8, 8, 8, 8,
  8, 8,
  8, 8,
  8, 8, 8, 8, 8, 8, 4,

  8, 8, 8, 8, 4,

  8,
  8, 8, 8, 8, 8, 8, 4,

  8, 8, 8, 8, 8, 4,
  8,
  8, 8, 8, 8, 4,
  8,
  8, 8, 8, 4,

  8, 8, 8, 8, 4,
  8, 8, 8, 8, 8,
  8, 8,

  8, 8,
  8, 8, 8, 16, 16, 16, 16,
  4,

  8, 8, 8,
  8, 8, 8,
  8, 4,
  4,

  8, 8, 8,
  8, 8, 8,
  8, 4,
  4,

  16, 16, 8, 8,
  8, 4, 8,
  8, 8, 4,

  8, 8, 8, 8,
  8, 8, 16, 16, 8, 4,

  8, 8, 8, 8,
  8, 16, 16, 8, 4,

  8, 8, 4,
  8, 8, 8,
  8, 8, 4,
  8, 8, 8,
  8, 16, 16, 8, 4, 2
};

void playSong() {
  int wholenote = (60000 * 4) / tempo;

  int notes = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < notes; i++) {
    int divider = durations[i];
    int noteDuration = wholenote / divider;

    if (melody[i] == REST) {
      noTone(BUZZER_PIN);
    } else {
      tone(BUZZER_PIN, melody[i], noteDuration * 0.9);
    }

    delay(noteDuration);
    noTone(BUZZER_PIN);
  }
}

void setup() {
  playSong();
}

void loop() {
  // Sürekli tekrar etsin istiyorsan:
  // playSong();
  // delay(1000);
}
