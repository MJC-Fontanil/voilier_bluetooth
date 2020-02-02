#include <Servo.h>

Servo gouvernail;  // variable du servomoteur pour le gouvernail
Servo voile;  // variable du servomoteur pour la voile

int position_gouvernail = 90;    // position gouvernail 0
int position_voile = 90;         // position voile 0

int angle_gouvernail[9] = {30, 45, 60, 75, 90, 115, 130, 145, 170}; //angles possibles pour le gouvernail
int angle_voile[9] = {45, 55, 65, 75, 90, 105, 115, 125, 135}; //angles possibles pour le gouvernail

int power[5] = {0, 10, 30, 50, 100}; //intensite des leds

char txt; // texte recu
char txt_prev; //texte recu envoi precedent

int ledG = 5; //led rouge gauche
int ledM = 11; //led du mat centrale
int ledD = 6; //led verte droite

void setup() {
  Serial.begin(9600); // ouvrir port serie
  while(!Serial); // attendre disponibilite du port serie
  gouvernail.attach(9);  // declarer pin gouvernail
  voile.attach(10);      // declarer pin voile
  
  pinMode(ledG, OUTPUT); // declarer pin led gauche
  analogWrite(ledG, 5); //allumer led gauche
  pinMode(ledM, OUTPUT); // declarer pin led milieu
  analogWrite(ledM, 10); //allumer led milieu
  pinMode(ledD, OUTPUT); // declarer pin led droite
  analogWrite(ledD, 10); //allumer led droite
  
  gouvernail.write(position_gouvernail); // reinitialiser la position du gouvernail
  voile.write(position_voile); // reinitialiser la position de la voile
 
  delay(1000); //attendre
  position_gouvernail = bougerServoDroite(gouvernail, position_gouvernail, angle_gouvernail[3]); //léger mouvement pour initialisation
  position_gouvernail = bougerServoDroite(gouvernail, position_gouvernail, angle_gouvernail[4]); //retour à 0

  position_voile = bougerServoDroite(voile, position_voile, angle_voile[3]); //léger mouvement pour initialisation
  position_voile = bougerServoDroite(voile, position_voile, angle_voile[4]); //retour à 0
}

void loop() {
   if (Serial.available() > 0) {
    // read the incoming byte:
    txt = Serial.read();

    Serial.print("I received: ");
    Serial.println(txt);
    if (txt >= '0' & txt <= '8' & txt_prev == 'G') { // verification signal dans plage de valeur
      int pos = txt - '0'; // conversion caractere en entier
      
      Serial.print("Signal recu :");
      Serial.println(pos);
      if(angle_gouvernail[pos] > position_gouvernail){
        position_gouvernail = bougerServoDroite(gouvernail, position_gouvernail, angle_gouvernail[pos]);
      } else if(angle_gouvernail[pos] < position_gouvernail){
        position_gouvernail = bougerServoGauche(gouvernail, position_gouvernail, angle_gouvernail[pos]);
      }
      Serial.print("Nouvel angle :");
      Serial.println(position_gouvernail);
    }
    
    if (txt >= '0' & txt <= '8' & txt_prev == 'V') { // verification signal dans plage de valeur
      int pos = txt - '0'; // conversion caractere en entier
      
      Serial.print("Signal recu :");
      Serial.println(pos);
      if(angle_voile[pos] > position_voile){
        position_voile = bougerServoDroite(voile, position_voile, angle_voile[pos]);
      } else if(angle_voile[pos] < position_voile){
        position_voile = bougerServoGauche(voile, position_voile, angle_voile[pos]);
      }
      Serial.print("Nouvel angle :");
      Serial.println(position_voile);
    }

    if (txt >= '0' & txt <= '4' & txt_prev == 'L') {
      int intensite = txt - '0'; // conversion caractere en entier
      analogWrite(ledG, power[intensite]/2); //allumer led gauche
      analogWrite(ledM, power[intensite]); //allumer led milieu
      analogWrite(ledD, power[intensite]); //allumer led droite
    }
    txt_prev = txt;
  }

}



int bougerServoDroite(Servo myservo, int position_actuelle, int angle){
  for (int pos = position_actuelle; pos <= angle; pos += 1) { // partir de 0 degrés vers l'angle max
    // in steps of 1 degree
    myservo.write(pos);
    delay(15);
  }
  return angle;
}

int bougerServoGauche(Servo myservo, int position_actuelle, int angle){
  for (int pos = position_actuelle; pos >= angle; pos -= 1) { // partir de 0 degrés vers l'angle min
    myservo.write(pos);
    delay(15);
  }
  return angle;
}
