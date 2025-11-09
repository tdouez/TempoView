
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <LoRa.h>
#include "payload.h"
#include "label_tic.h"
#include <Preferences.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define VERSION   "1.0.0"

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        8 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS  56 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 1 // Time (in milliseconds) to pause between pixels

enum _info {
  HC,
  HP,
  ROUGE,
  BLANC,
  BLEU,
  SANS
};

const long frequency = 868E6; // LoRa Frequency
const int csPin = 7;          // LoRa radio chip select
const int resetPin = 3;       // LoRa radio reset
const int irqPin = 2;         // change for your board; must be a hardware interrupt pin

uint8_t couleur_demain = SANS;
uint8_t periode = SANS;

bool flag_tic_standard = false;
bool flag_associated = false;
uint64_t cpt_id=0;
PayloadData payload_data;
Preferences preferences;

void afficheUnite(unsigned int nbr, uint32_t color, unsigned int offset)
{
  switch (nbr) {

    case 0:
      pixels.setPixelColor(1+offset, color);
      pixels.setPixelColor(2+offset, color);
      pixels.setPixelColor(3+offset, color);
      pixels.setPixelColor(4+offset, color);
      pixels.setPixelColor(5+offset, color);
      pixels.setPixelColor(6+offset, color);
      break;
      
    case 1:
      pixels.setPixelColor(2+offset, color);
      pixels.setPixelColor(3+offset, color);
      break;

    case 2:
      pixels.setPixelColor(0+offset, color);
      pixels.setPixelColor(1+offset, color);
      pixels.setPixelColor(2+offset, color);
      pixels.setPixelColor(4+offset, color);
      pixels.setPixelColor(5+offset, color);
      break;

    case 3:
      pixels.setPixelColor(0+offset, color);
      pixels.setPixelColor(1+offset, color);
      pixels.setPixelColor(2+offset, color);
      pixels.setPixelColor(3+offset, color);
      pixels.setPixelColor(4+offset, color);
      break;

    case 4:
      pixels.setPixelColor(0+offset, color);
      pixels.setPixelColor(2+offset, color);
      pixels.setPixelColor(3+offset, color);
      pixels.setPixelColor(6+offset, color);
      break;

    case 5:
      pixels.setPixelColor(0+offset, color);
      pixels.setPixelColor(1+offset, color);
      pixels.setPixelColor(3+offset, color);
      pixels.setPixelColor(4+offset, color);
      pixels.setPixelColor(6+offset, color);
      break;

    case 6:
      pixels.setPixelColor(0+offset, color);
      pixels.setPixelColor(3+offset, color);
      pixels.setPixelColor(4+offset, color);
      pixels.setPixelColor(5+offset, color);
      pixels.setPixelColor(6+offset, color);
      break;

    case 7:
      pixels.setPixelColor(1+offset, color);
      pixels.setPixelColor(2+offset, color);
      pixels.setPixelColor(3+offset, color);
      break;

    case 8:
      for(int i=0; i<7; i++) pixels.setPixelColor(i+offset, color);
      break;

    case 9:
      pixels.setPixelColor(0+offset, color);
      pixels.setPixelColor(1+offset, color);
      pixels.setPixelColor(2+offset, color);
      pixels.setPixelColor(3+offset, color);
      pixels.setPixelColor(4+offset, color);
      pixels.setPixelColor(6+offset, color);
      break;
  }
}

void afficheInfo(void)
{
  switch (periode) {
    case SANS:
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.setPixelColor(1, pixels.Color(0, 0, 0));
      pixels.setPixelColor(2, pixels.Color(0, 0, 0));
      pixels.setPixelColor(5, pixels.Color(0, 0, 0));
      pixels.setPixelColor(6, pixels.Color(0, 0, 0));
      break;

    case HC:
      pixels.setPixelColor(0, pixels.Color(128, 128, 0));
      pixels.setPixelColor(1, pixels.Color(128, 128, 0));
      pixels.setPixelColor(2, pixels.Color(0, 0, 0));
      pixels.setPixelColor(5, pixels.Color(0, 0, 0));
      pixels.setPixelColor(6, pixels.Color(128, 128, 0));
      break;

    case HP:
      pixels.setPixelColor(0, pixels.Color(128, 128, 0));
      pixels.setPixelColor(1, pixels.Color(128, 128, 0));
      pixels.setPixelColor(2, pixels.Color(128, 128, 0));
      pixels.setPixelColor(5, pixels.Color(128, 128, 0));
      pixels.setPixelColor(6, pixels.Color(128, 128, 0));
      break;
  }

  switch (couleur_demain) {

    case SANS:
      pixels.setPixelColor(4, pixels.Color(0, 0, 0));
      break;

    case BLEU:
      pixels.setPixelColor(4, pixels.Color(0, 0, 200));
      break;
    
    case BLANC:
      pixels.setPixelColor(4, pixels.Color(150, 150, 150));
      break;

    case ROUGE:
      pixels.setPixelColor(4, pixels.Color(150, 0, 0));
      break;
  }

}

void affichePoint(uint32_t color)
{
  pixels.setPixelColor(3, color);
  pixels.show();
}

void afficheNombre(unsigned long nbr, uint32_t color)
{
  pixels.clear();

  if (nbr == 0) {
    afficheUnite(nbr, color, 7);
  }
  else {
    int column = 0;
    while (nbr > 0) {
      int digit = nbr % 10;
      afficheUnite(digit, color, ((column++)*7)+7);
      nbr /= 10;
    }
  }

  afficheInfo();

  pixels.show();
}

// ---------------------------------------------------------------- 
// onReceive
// ---------------------------------------------------------------- 
void onReceive(int packetSize) {

  uint8_t buffer[sizeof(payload_data)]; 
  char buff_value[PAYLOAD_VALUE_LEN];
  bool flag = true;

  /*if (packetSize >0) {
    Serial.print(packetSize);
    Serial.print("/");
    Serial.println(sizeof(PayloadData));
  }*/

  if (packetSize == sizeof(PayloadData)) {

    LoRa.readBytes(buffer, packetSize);

    if (buffer[0] == PREABLE_VALUE) {
      if (payload_deserialize(&payload_data, buffer, packetSize)) {
        affichePoint(pixels.Color(0, 100, 0));
        //if (flag_associated == true) {
          payload_get_value_str(&payload_data, buff_value);
          //updateTICEntry(payload_data.label_id, buff_value);
          if (payload_data.sender_id == LORALINK_S) flag_tic_standard = true;
            else flag_tic_standard = false;

          if (payload_data.label_id == 0x1A) afficheNombre(atol(buff_value), pixels.Color(0, 100, 0));
        affichePoint(pixels.Color(0, 0, 0));
       /*}
        else {
          if (payload_data.label_id == 0xFF) {
            flag_associated = true;
            cpt_id = payload_get_counter_addr_toUint64(&payload_data);
            preferences.begin("fbs", false);
            preferences.putULong64("CPTID", cpt_id);
            preferences.end();

            //clignoteLeds(4, 200);
          }
        }*/
      }
      else {
        affichePoint(pixels.Color(150, 0, 0));
        Serial.println(F("❌ CRC invalide"));
        //clignoteLed(ledErrPin, 3, 200);
        flag = false;
        affichePoint(pixels.Color(0, 0, 0));
      }
    }
    else {
      affichePoint(pixels.Color(150, 0, 150));
      Serial.println(F("❌ Entete invalide"));
      //clignoteLed(ledErrPin, 4, 100);
      flag = false;
      affichePoint(pixels.Color(0, 0, 0));
    }
  }
}

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  Serial.begin(115200); // mode historique
  delay(1000);

  Serial.println(F(" ███████╗██████╗ ███████╗"));
  Serial.println(F(" ██╔════╝██╔══██╗██╔════╝"));
  Serial.println(F(" █████╗  ██████╔╝███████╗"));
  Serial.println(F(" ██╔══╝  ██╔══██╗╚════██║"));
  Serial.println(F(" ██║     ██████╔╝███████║"));
  Serial.println(F(" ╚═╝     ╚═════╝ ╚══════╝"));
  Serial.print(F(" TempoView  v"));
  Serial.println(VERSION);

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  // Demarrage module Lora ------
  Serial.print(F("Init LoRa : "));
  LoRa.setPins(csPin, resetPin, irqPin);
  if (LoRa.begin(frequency)) {
    LoRa.setSpreadingFactor(7);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(4);
    LoRa.setPreambleLength(8);
    LoRa.enableCrc();
    LoRa.setTxPower(6);
	  LoRa.idle();
	
	  Serial.println(F("ok"));
  }
  else {
    Serial.println(F("ko!"));
  }

}

void loop() {

  onReceive(LoRa.parsePacket());

}
