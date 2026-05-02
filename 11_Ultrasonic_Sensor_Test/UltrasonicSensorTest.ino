/*
  PROJE: Ultrasonik Sensör Testi
  AÇIKLAMA: HC-SR04 ultrasonik sensörü test kodu (pilot/tamamlanmamış)
  Trig pini pulse gönderme mantığı doğru ama ölçüm henüz programlanmamış
  KULLANILAN BILEŞENLER: HC-SR04 ultrasonik sensörü
  NOTLAR: Tamamlanmayan deneme projesi
*/

#define trig 6
#define echo 5
void setup() {
  pinMode(trig, OUTPUT);
  pinmode(echo, INPUT);
}

void loop() {
  digitalWrite(trig, LOW);
  delay(1);
  digitalWrite(trig, HIGH);
  delay(1);
  digitalWrite(trig, LOW);
  delay(1);
}
