/*
  PROJE: Motor Temel Test
  AÇIKLAMA: 2 kanallı DC motor hız ve yön kontrolü testleri
  Motorlar için ileri gitme, durma ve sağa dönme hareketleri yapılır
  KULLANILAN BILEŞENLER: 2x DC motor, L298N motor sürücü
  NOTLAR: Basit test ve deney amaçlı
*/

const int ENA = 5;
const int ENB = 6;
const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;

int hizSol = 180;
int hizSag = 180;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  ileriGit();
  delay(500);

  dur();
  delay(500);

  saga90Don();
  delay(500);

  // Bu döngü sürekli tekrar eder
}

void ileriGit() {
  analogWrite(ENA, hizSol);
  analogWrite(ENB, hizSag);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void saga90Don() {
  analogWrite(ENA, hizSol);
  analogWrite(ENB, hizSag);

  // Araç kendi etrafında sağa döner
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  delay(650); // Bunu 90 dereceye göre ayarlayacaksın

  dur();
}

void dur() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
