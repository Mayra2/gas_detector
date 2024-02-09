/*Uključivanje biblioteka funkcija za display i melodije buzzera*/
#include <LiquidCrystal_I2C.h>
#include "pitches.h"

const int trigPin = 9;
const int echoPin = 10;//pinovi za senzor pokreta
long udaljenost;
long trajanje; //varijable za racunanje udaljenosti i trajanja pokreta

LiquidCrystal_I2C lcd(0x27, 16, 2); //definisanje varijable lcd za koristenje u nastavku
int val = A0; //input za senzor plina
int MAX = 600; //max vrijednost plina do koje ga senzor ignorise

int buzzerPin = 13; //pin na kom se nalazi + buzzera
const int buttonPin = 7; //pin na kom se nalazi taster

/*Funkcija sigurno poziva se kada je stanje sistema sigurno, tj. kada nema pokreta ni plina.
Postavlja boju diode na zelenu, isključuje ton buzzera i na displayu se ispisuje da je siguran sistem.*/
void sigurno(){
  noTone(buzzerPin);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  lcd.clear();
  lcd.println("Sistem siguran");
}

/*Funkcija button poziva se kada je taster pritisnut.
Iskljucuje buzzer, postavlja boju diode na zelenu i na display ispisuje korisnikov prekid alarma. 
Nakon toga čeka 2 sekunde i sistem vraća u sigurno stanje.*/
void button(){
  digitalWrite(buzzerPin, HIGH);
    noTone(buzzerPin);
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      lcd.clear();
      lcd.println("Korisnicki prekinuto");
      delay(3000);
      sigurno();
}
void setup()
{
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  pinMode(val, INPUT);
  pinMode(3, OUTPUT); //pin za zelenu boju
  pinMode(4, OUTPUT);//pin za plavu boju
  pinMode(5, OUTPUT);//pin za crvenu boju
  pinMode(6, OUTPUT); //dovod napajanja na diodu
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(6, HIGH);
}
void loop()
{
  val = analogRead(A0);
  digitalWrite(trigPin, LOW);
  delay(30);
  digitalWrite(trigPin, HIGH);
  delay(30);
  digitalWrite(trigPin, LOW);
  trajanje = pulseIn(echoPin, HIGH); //račun trajanja i udaljenosti
  udaljenost = trajanje * 0.034 / 2;


if (val >= MAX) { //ako je senzor očitao veći nivo plina od MAX
    digitalWrite(3,HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW); //postavlja crvenu boju diode
    digitalWrite(buzzerPin, LOW);
    tone(buzzerPin, 443, trajanje); //definiše melodiju buzzera i trajanje kao trajanje očitavanja
   
    if(digitalRead(buttonPin)==LOW){   //ako je pritisnut taster poziva funkciju button
      button();
    }
       
    lcd.clear();
    lcd.println("Pozor! Plin");
 }else if(val<MAX && udaljenost < 10){ //ako je senzor očitao manji nivo plina od MAX, ali postoji pokret
    digitalWrite(5, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH);
   digitalWrite(buzzerPin, LOW);
    tone(buzzerPin, 443, trajanje);

    if(digitalRead(buttonPin)==LOW){   
      button();
    }
       
    lcd.clear();
    lcd.println("Pokret");
} else{ //u slučaju da nema ni pokreta ni plina, stanje sistema je sigurno
   sigurno();
}
}