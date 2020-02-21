#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
/*
 * ----------------------------------------------------------------------------
 * This sketch uses the MFRC522 library ; see https://github.com/miguelbalboa/rfid
 * for further details and other examples.
 * 
 * NOTE: The library file MFRC522.h has a lot of useful info. Please read it.
 * 
 * This sketch show a simple locking mechanism using the RC522 RFID module.
 * ----------------------------------------------------------------------------
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 */

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for 16x2 LCD display
String read_rfid;                   
String ok_rfid_1="9daff87";        // keyfob RFID ID 
String ok_rfid_2="b66311f9";       // card   RFID ID

/*
 * Initialize.
 */

 // variables to hold the new and old switch states
int rel = 7;
int rel2 = 8;



void setup() {
    Serial.begin(9600);         // Initialize serial communications with the PC
   // while (!Serial);            // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();                // Init SPI bus
    mfrc522.PCD_Init();         // Init MFRC522 card

 lcd.init();                      // initialize the lcd 
    lcd.setCursor(0,0);          
  lcd.print("RFID SCANNER");
   lcd.backlight();
   delay (2000);
  lcd.noBacklight();
 

  pinMode(rel, OUTPUT );  // relay1 output 
  pinMode(rel2, OUTPUT ); // relay2 output 
 digitalWrite(rel, HIGH); // using 5V 2 Relay Module opto-isolated LOW = ON / HIGH = OFF
 digitalWrite(rel2, HIGH); //


 
}

/*
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    read_rfid="";
    for (byte i = 0; i < bufferSize; i++) {
        read_rfid=read_rfid + String(buffer[i], HEX);
    }
}


void noaccess()  //
{
 lcd.clear();
  lcd.setCursor(0,0);
 lcd.print("UNKNOWN RFID TAG"); 
//lcd.print(read_rfid); ///CARD ID
 lcd.setCursor(0,1);
    lcd.print(" ACCESS DENIED  ");
     for (int i = 0; i <= 10; i++){
 lcd.backlight();
delay (100);
 lcd.noBacklight();
 delay (100);
    }

lcd.noBacklight();
 lcd.clear();

  return;
}

void loop() {


      // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;

     dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
     Serial.println((String)"card_id: "+read_rfid);
    Serial.println("card id  [read_rfid]");
    if (read_rfid==ok_rfid_1) {    
  digitalWrite(rel, LOW);
  lcd.setCursor(0,0);
   lcd.print("     KEYFOB    ");
    lcd.setCursor(0,1);
    lcd.print(" ACCESS GRANTED  ");
  lcd.backlight();
    delay (1500);
    }

 else  if (read_rfid==ok_rfid_2) {
 digitalWrite(rel2, LOW);
  lcd.setCursor(0,0);
   lcd.print("     CARD      ");
    lcd.setCursor(0,1);
    lcd.print(" ACCESS GRANTED  ");
  lcd.backlight();
    delay (1500);
    }

         

else 

  {
  noaccess();  
  }


 digitalWrite(rel, HIGH); // TURN OFF OUTPUTS 
 digitalWrite(rel2, HIGH);

lcd.noBacklight(); //TURN OFF LCD BACKLIGHT
    lcd.clear();

}





 
