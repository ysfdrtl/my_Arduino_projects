/*
  PROJE: Rastgele LED Seçici
  AÇIKLAMA: Pin 8 butonuna basıldığında, 5 farklı LED (PIN 2-6) üzerinden
  rastgele birini seçerek yakar, diğerlerini söndürür
  KULLANILAN BILEŞENLER: Buton (push-button), 5x LED
  NOTLAR: random() fonksiyonu örneği
*/

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(A0, INPUT);

  pinMode(8, INPUT);

  randomSeed(analogRead(0));
}

int tmp = 2;
void loop() {
  int rnd = random(300);
  rnd = (rnd % 5) + 2;
  if (digitalRead(8)) {
    digitalWrite(tmp, LOW);
    tmp = rnd;
    digitalWrite(rnd, HIGH);
    while (digitalRead(8)) {
      delay(50);
    }
  }

}
