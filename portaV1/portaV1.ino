#include <EEPROM.h>     // We are going to read and write PICC's UIDs from/to EEPROM
#include <SPI.h>        // RC522 Module uses SPI protocol
#include <MFRC522.h>  // Library for Mifare RC522 Devices


#define COMMON_ANODE

#ifdef COMMON_ANODE
#define LED_ON LOW
#define LED_OFF HIGH
#else
#define LED_ON HIGH
#define LED_OFF LOW
#endif



struct Pessoa{
  String nome;
  byte ID[4];
};

//uint8_t NmaxPessoa = 6; //numero maximo de pessoas
//Pessoa pessoas[NmaxPessoa];

//uint8_t numPessoas = 0;
constexpr uint8_t ledpinG = 27;   //pino do led verde
constexpr uint8_t ledpinR = 26;   //pino do led vermelho
constexpr uint8_t ledpinB = 14;   //pino do led vermelho
constexpr uint8_t pinMovmodulo = 36; // pino do modulo de movimento
constexpr uint8_t pinDadosPorta = 25; // pino do dados da porta/reler

byte readCard[4];   // Stores scanned ID read from RFID Module

constexpr uint8_t RST_PIN = 2;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 5;     // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);

///////////////////////////////////////// Setup ///////////////////////////////////
void setup() {
  //Protocol Configuration
  Serial.begin(9600);  // Initialize serial communications with PC
  SPI.begin();           // MFRC522 Hardware uses SPI protocol
  mfrc522.PCD_Init();    // Initialize MFRC522 Hardware

  pinMode(ledpinR, OUTPUT);
  pinMode(ledpinG, OUTPUT);
  pinMode(ledpinB, OUTPUT);
  pinMode(pinDadosPorta, OUTPUT);
  pinMode(pinMovmodulo, INPUT);
  digitalWrite(ledpinR, HIGH);
  digitalWrite(ledpinB, HIGH);
  digitalWrite(ledpinG, HIGH);
}

///////////////////////////////////////// Main Loop ///////////////////////////////////
void loop () {
  getID();

}

///////////////////////////////////////// Get PICC's UID ///////////////////////////////////
uint8_t getID() {
  // Getting ready for Reading PICCs
  if ( !mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( !mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  //Serial.println(F("UID do chip lido:"));
  for ( uint8_t i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
    //Serial.println(readCard[i], HEX);
    //nova.ID[i] = readCard[i];
  }
  Serial.print("bem Vindo ");
  //Serial.print(nova.nome);
  Serial.print(" seu id é ");
  for(uint8_t i=0;i<4;i++){
    //Serial.print(nova.ID[i], HEX);
  }
  Serial.println("");
  abrePorta();
  return 1;
}
void abrePorta(){
  Serial.println("porta aberta");
  digitalWrite(pinDadosPorta,HIGH);
  digitalWrite(ledpinG, LOW);
  delay(3330);
  digitalWrite(ledpinG, HIGH);

  digitalWrite(ledpinB, LOW);
  delay(3330);
  digitalWrite(ledpinB, HIGH);

  digitalWrite(ledpinR, LOW);
  delay(3330);
  digitalWrite(ledpinR, HIGH);

  delay(100);
  Serial.println("porta fechada");
  digitalWrite(pinDadosPorta, LOW);
}
/*
void cadastrarPessoa(String nome, byte* ID) {
  if (numPessoas < NmaxPessoa) {
    //pessoas[numPessoas].nome = nome;
    for (uint8_t i = 0; i < 4; i++) {
      pessoas[numPessoas].ID[i] = ID[i];
    }
    numPessoas++;
  }
}
*/