/* == Partie application (Roland) */

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
String message;

/* ========== */

/* == Partie RFID (Roland) */

// lien aide: https://www.electronique-mixte.fr/microcontrolleurs/rfid-controle-dacces-par-badge-avec-arduino/

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
    
#define Max_Acces 3

byte Count_acces=0; 
byte CodeVerif=0; 
byte Code_Acces[4]={0x12, 0x63, 0x6E, 0x1A};             //  ID DE LA CARTE (IL FAUT CHANGER LES NUMEROS APRES LES "x" CORRESPONDANT A LA CARTE/BADGE )

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

// Init array that will store new NUID 
byte nuidPICC[4];

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

/* == Partie Lumière (Enzo) */

int lum1 = 13;
int lum2 = 8;
int lum3 = 7;

/* ========== */


void setup() {

  // ENZO 
  pinMode(lum1, OUTPUT);
  pinMode(lum2, OUTPUT);
  pinMode(lum3, OUTPUT);

  // OSCAR
  pinMode(entree, INPUT);
  pinMode(sortie, INPUT);

  // ROLAND (APPLICATION)
  Serial.begin(9600);
  pinMode(9,OUTPUT); 
  digitalWrite(9,HIGH);
  Serial.println("Enter AT commands:");
  mySerial.begin(9600);

  // ROLAND RFID
  SPI.begin(); 
  rfid.PCD_Init();
  
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

/* == Partie RFID (Roland) == */

void getBadge(){

  // Initialisé la boucle si aucun badge n'est présent 
  if ( !rfid.PICC_IsNewCardPresent())
    return;

  // Vérifier la présence d'un nouveau badge 
  if ( !rfid.PICC_ReadCardSerial())
    return;

  // Afffichage 
  Serial.println(F("Un badge est détecté"));

  // Enregistrer l’ID du badge (4 octets) 
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }

  // Vérification du code 
  CodeVerif = GetAccesState(Code_Acces,nuidPICC); 
  if (CodeVerif != 1)
  {
    Count_acces+=1;
    if(Count_acces==Max_Acces)
    {
     // Dépassement des tentatives (clignotement infinie) 
     while(1)
     { 
      // Affichage 
      Serial.println("Alarme!");
     }
    }
    else
    {
      // Affichage 
      Serial.println("Code érroné");
    }
  }
  else
  {
    // Affichage 
    Serial.println("Ouverture de la porte");
    Count_acces=0; 
  }

  // Affichage de l'identifiant 
  Serial.println(" L'UID du tag est:");
  for (byte i = 0; i < 4; i++) 
  {
    Serial.print(nuidPICC[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Re-Init RFID
  rfid.PICC_HaltA(); // Halt PICC
  rfid.PCD_StopCrypto1(); // Stop encryption on PCD
}

byte GetAccesState(byte *CodeAcces,byte *NewCode) {
  byte StateAcces=0; 
  if ((CodeAcces[0]==NewCode[0])&&(CodeAcces[1]==NewCode[1])&&
  (CodeAcces[2]==NewCode[2])&& (CodeAcces[3]==NewCode[3]))
    return StateAcces=1; 
  else
    return StateAcces=0; 
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
