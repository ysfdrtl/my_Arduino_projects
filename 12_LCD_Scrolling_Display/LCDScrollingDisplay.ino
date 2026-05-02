/*
  PROJE: LCD Hareketli Yazı Gösterimi
  AÇIKLAMA: 16x2 LCD ekranında Türkçe yazı ("belki üstümüzden bir kuş geçer...")
  sırayla kaydırarak gösterir (scrolling effect)
  Metin sırayı tekrar eder
  KULLANILAN BILEŞENLER: 16x2 LCD ekran
  NOTLAR: LiquidCrystal kütüphanesi kullanılır
*/

#include <LiquidCrystal.h>
#include <string.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char metin[] = "belki ustumuzden bir kus gecer, kanadindan bir tuy duser.  ";
char metins[17];

void setup() {
  lcd.begin(16, 2);
}

void loop() {
  int uzunluk = strlen(metin);

  for (int j = 0; j < uzunluk; j++) {
    for (int i = 0; i < 16; i++) {
      metins[i] = metin[(i + j) % uzunluk];
    }
    metins[16] = '\0';

    printslowly(metins);
  }
}

void printslowly(char* yaz) {
  lcd.clear();
  lcd.print(yaz);
  delay(200);
}
