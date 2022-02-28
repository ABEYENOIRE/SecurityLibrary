/* == Partie application (Roland) */

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
String message;

/* ========== */

/* == Partie Détecteur de passage (Oscar) */

int entree = 4;
int etat1;    // variable pour létat actuel
int etatav1 = 0;  // variable pour l'état précédent

int sortie = 5;
int etat2;
int etatav2 = 0;

int nbrpers = 0;

/* ========== */


int lum1 = 13;
int lum2 = 8;
int lum3 = 7;


void setup() {
  pinMode(lum1, OUTPUT);
  pinMode(lum2, OUTPUT);
  pinMode(lum3, OUTPUT);

  pinMode(entree, INPUT);
  pinMode(sortie, INPUT);
  
  Serial.begin(9600);
  pinMode(9,OUTPUT); 
  digitalWrite(9,HIGH);
  Serial.println("Enter AT commands:");
  mySerial.begin(9600);
}

void loop()
{
  if (mySerial.available()){
    Serial.write(mySerial.read());  
  }
  
  
  if (Serial.available()){
      mySerial.write(Serial.read());
  }

  etat1 = digitalRead(entree);
  etat2 = digitalRead(sortie);
  comptage(etat2, etat1);
  

  while(mySerial.available()){

    message = mySerial.readString();

    //int valPassage = analogRead(entree);
    //Serial.println(valPassage);
    
    if(message == "LumOff"){

      lumOff();
      
    }

    if(message == "LumOn"){

      lumOn();
      
    }
    
  }
  
}

/* == Partie Détecteur de passage (Oscar) == */

void comptage(int sortie, int entree){
  
if (entree != etatav1){

  etatav1 = entree;

  if(entree == 0){

    nbrpers++;
    
    Serial.println("⚠ | Une personne est entrée dans la pièce !");
    Serial.print("Nombre de personne : ");
    Serial.println(nbrpers);
    Serial.println("");

    delay(2000);
    
  }
      
}

if(sortie != etatav2){

  etatav2 = sortie;

  if(sortie == 0){

    if(nbrpers > 0){

      Serial.println("⚠ | Une personne est sortie !");
      nbrpers--;
      
    }else{

      Serial.println("-- Impossible, il n'y avait déjà aucune personne dans la pièce --");
      
    }

    Serial.print("Nombre de personne : ");
    Serial.println(nbrpers);
    Serial.println("");

    delay(2000);
    
  }
  
}
  
}

/* ========== */


/* == Partie Lumières (Enzo) == */

void lumOn(){
  
  Serial.println("Lumières allumées");
  digitalWrite(lum1, HIGH);
  digitalWrite(lum2, HIGH);
  digitalWrite(lum3, HIGH);
  delay(1000);
  
}

void lumOff(){
  
  Serial.println("Lumières éteintes");
  digitalWrite(lum1, LOW);
  digitalWrite(lum2, LOW);
  digitalWrite(lum3, LOW);
  delay(1000);
  
}

/* ========== */
