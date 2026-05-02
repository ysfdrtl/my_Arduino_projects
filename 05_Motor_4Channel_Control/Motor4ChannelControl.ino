/*
  PROJE: 4 Kanallı Motor Kontrolü
  AÇIKLAMA: 4 DC motor (2 üst, 2 alt seviye) kontrolü ve test
  Motorlar ücü (elektrik, mekanik vb.) konfigürasyonları kontrol ederken test edilir
  KULLANILAN BILEŞENLER: 4x DC motor, L298N motor sürücü
  NOTLAR: 8 pinli (4 PWM + 4 yön) konfigürasyon
*/

#define A2U 22
#define A1U 24

#define B1U 26
#define B2U 28

#define A2A 30
#define A1A 32

#define B1A 34
#define B2A 36


#define pwmAU 11
#define pwmBU 10

#define pwmAA 9
#define pwmBA 8


void setup() {
  for (int i = 8; i < 12; i++) {
    pinMode(i, OUTPUT);
  }

  for (int i = 22; i < 38; i += 2) {
    pinMode(i, OUTPUT);
  }
  analogWrite(pwmAU, 255);
  analogWrite(pwmBU, 255);
  analogWrite(pwmAA, 255);
  analogWrite(pwmBA, 255);

}

void loop() {
  ileri();
  delay(1000);
  dur();
  delay(500);
  geri();
  delay(1000);
  dur();
  delay(500);

}

void ileri() {

  digitalWrite(A1U, HIGH);
  digitalWrite(B1U, HIGH);
  digitalWrite(A1A, HIGH);
  digitalWrite(B1A, HIGH);

  digitalWrite(A2U, LOW);
  digitalWrite(B2U, LOW);
  digitalWrite(A2A, LOW);
  digitalWrite(B2A, LOW);
}

void geri() {

  digitalWrite(A1U, LOW);
  digitalWrite(B1U, LOW);
  digitalWrite(A1A, LOW);
  digitalWrite(B1A, LOW);

  digitalWrite(A2U, HIGH);
  digitalWrite(B2U, HIGH);
  digitalWrite(A2A, HIGH);
  digitalWrite(B2A, HIGH);
}

void dur() {

  digitalWrite(A1U, LOW);
  digitalWrite(B1U, LOW);
  digitalWrite(A1A, LOW);
  digitalWrite(B1A, LOW);


  digitalWrite(A2U, LOW);
  digitalWrite(B2U, LOW);
  digitalWrite(A2A, LOW);
  digitalWrite(B2A, LOW);
}
