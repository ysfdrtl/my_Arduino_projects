/*
  PROJE: 2 Kanallı Motor Kontrol Testi
  AÇIKLAMA: 2 DC motor ile temel ileri hareketi test etme
  Motorlar 2 saniye ileri gider, sonra durur ve döngü tekrar eder
  KULLANILAN BILEŞENLER: 2x DC motor, L298N motor sürücü
  NOTLAR: Basit ve hızlı test kodu
*/

#define pwma 2
#define a2 22
#define a1 23
#define pwmb 3
#define b1 24
#define b2 25

void setup() {
  pinMode(pwma,OUTPUT);
  pinMode(pwmb,OUTPUT);
  pinMode(a1,OUTPUT);
  pinMode(a2,OUTPUT);
  pinMode(b1,OUTPUT);
  pinMode(b2,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(pwma,170);
  analogWrite(pwmb,170);

  digitalWrite(a1,HIGH);
  digitalWrite(a2,LOW);
  digitalWrite(b1,HIGH);
  digitalWrite(a2,LOW);
  delay(2000);
  
  digitalWrite(a1,LOW);
  digitalWrite(a2,LOW);
  digitalWrite(b1,LOW);
  digitalWrite(a2,LOW);
  delay(2000);
}
