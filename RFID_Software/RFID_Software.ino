/* STEPS OF SOFTWARE DEVELOPMENT
 * -------------------------------------------
 *          Including the librairies
 * -------------------------------------------
 * * RFID LIBRAIRIES (SPI, MFRC522, EEPROM)
 * * SERVO LIBRAIRIES (Servo)
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <EEPROM.h>

/* -------------------------------------------
 * PINS CONFIGURATION AND INSTANCE DECLARATION
 * -------------------------------------------
*/

//RFID DEFINITION
#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
MFRC522::MIFARE_Key *key;
byte tagUid[4]; // Définition du tableau d'octet qui contient l'UID du tag

//SERVO DEFINITION
#define servoPin 6 // Pin à relier à la sortie d'arduino

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int angle = 0;    // variable to store the servo position

// BUZZER AND LED DEFINITION FOR SIGNALISATION
#define Buz_OUT 7
#define RED_LED 5
#define GREEN_LED 4
#define BLUE_LED 3


  // BASE DE DONNEES D'IDs
  byte save_Uid[4] = {0xBC, 0xCD, 0x56, 0x49};
  
void setup(){
/* -------------------------------------------
 *              INITIALISATIONS
 * -------------------------------------------
 * RFID INITIALISATION
 * SYSTEMS ON SIGNALISATION (Blue LED)
 * CURRENT SIGNALISATIONS (Red and Green Led and buzzer)
 * SERVO INITIALISATION
 * ENREGISTREMENT D'UN BADGE
*/
  // SIGNALISATION'S INITIALISATION
  pinMode(Buz_OUT, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  //digitalWrite(BLUE_LED, HIGH);

  // RFID INITIALISATION
  Serial.begin(9600);    // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  bool Com_Success = mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  digitalWrite(BLUE_LED, Com_Success);
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  // SERVO INITIALISATION
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  for (angle = 0; angle <= 90; angle += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(angle);              // tell servo to go to position in variable 'pos'
    delay(15);
  }
  set_Uid_Null();
}
void loop(){
  get_uid();
  int count = 0;
  for(int k = 0; k < 4; k++){
    if(tagUid[k] == save_Uid[k]) count++;
  }
  switch(count){
    case 4:
      notif(GREEN_LED, 600);
      delay(300);
      open_Gate();
      break;
    default:
      int Null_UId = Uid_Null();
      switch(Null_UId){
        case 4:
          break;
        default:
          notif(RED_LED, 100);
          delay(300);
          break;        
      }
    break;
  } 
  set_Uid_Null();
}

/* Functions DEFINITION
 * GETTING RFID IDs
 * NOFICATIONS
 * ACTIVATION OF SERVO
 */

 
 // GETTING IDs 

 void get_uid()
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  Serial.print("UID : ");
  for (int j = 0; j < mfrc522.uid.size;j++)
  {
    tagUid[j] = mfrc522.uid.uidByte[j];
    Serial.print(tagUid[j], HEX);
  }
  mfrc522.PICC_HaltA();
  Serial.println();
  //return;

  // Dump debug info about the card; PICC_HaltA() is automatically called
  // mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}

// REFRESH UID CONTAINER
void set_Uid_Null(){
  for (int j = 0; j < 4; j++)
  {
    tagUid[j] = 0x00;
  }
}

int Uid_Null(){
  int compt = 0;
  for (int j = 0; j < 4; j++)
  {
    if(tagUid[j] == 0x00) compt++;
  }
  return compt;
}

// OUVERTURE PORTE

void open_Gate()
{
  for (angle = 90; angle >= 0; angle -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(angle);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(1000);
  for (angle = 0; angle <= 90; angle += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(angle);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

// NOTIFICATION
void notif(int Led_Pin, int freq)
{
   digitalWrite(Led_Pin, HIGH);
   tone(Buz_OUT, freq, 300); // allume le buzzer actif arduino
   delay(300);
   digitalWrite(Led_Pin, LOW);
}
