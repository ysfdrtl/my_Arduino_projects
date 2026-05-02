
/*
  PROJE: Buton Kontrollü LED Açılıp Kapanış
  AÇIKLAMA: Pin 8 butonuna basıldığında LED13 (PIN 13) açılıp kapatılır
  Buton basılı iken LED durumunu değiştirir
  KULLANILAN BILEŞENLER: Buton (push-button), LED
  NOTLAR: Basit giriş/çıkış demo projesi
*/

bool Yazan = 0;
void setup() {
  pinMode(13, OUTPUT);

  pinMode(8, INPUT_PULLUP);
  //pinMode(9, INPUT_PULLUP);
}

void loop() {
  if (!digitalRead(8)) {
    Yazan = !Yazan;
    digitalWrite(13, Yazan);
    while (!digitalRead(8))
    {
      delay(1);
    }
  }

}
