#include <Arduino.h>
#include "ui/lv_setup.h"
#include "ui/ui.h"
#include "pin_config.h"
#include <SPI.h>
#include <LoRa.h>
#include "payload.h"
//#include "label_tic.h"
#include <Preferences.h>

#define VERSION   "1.0.0"

#define LORA_SF					7
#define LORA_BW					125E3		
#define LORA_CR					4
#define LORA_PREAMBLE_LENGTH	8
#define LORA_POWER			6

unsigned long start = millis();
unsigned i=0;

const long frequency = 868E6; // LoRa Frequency

const int bpPin = 14;         // push button


#define SS      10
#define RST     12  // pas toujours utilisé
#define DIO0    13

#define SCK     1
#define MOSI    3
#define MISO    2

#define TARIF_AUTRE 0
#define TARIF_TEMPO 1

#define HEURE_TOUTE    0
#define HEURE_CREUSE   1
#define HEURE_PLEINE   2

#define JOUR_SANS  0
#define JOUR_BLEU  1
#define JOUR_BLANC 2
#define JOUR_ROUGE 3


uint8_t tarif = TARIF_AUTRE;
uint8_t couleur_demain = JOUR_SANS;
uint8_t jour = JOUR_SANS;
uint8_t heure = HEURE_TOUTE;
uint8_t demain = JOUR_SANS;

uint32_t puissance_max=0;
bool flag_tic_standard = false;
bool flag_associated = false;
uint64_t cpt_id=0;
PayloadData payload_data;
Preferences preferences;
SPIClass mySPI(FSPI);

lv_color_t color_white = lv_color_hex(0xffffff);
lv_color_t color_grey = lv_color_hex(0x747171);
lv_color_t color_red = lv_color_hex(0xc8471a);
lv_color_t color_blue = lv_color_hex(0x386a77);
lv_color_t color_purple = lv_color_hex(0xec04ff);
lv_color_t color_black = lv_color_hex(0x000000);
lv_color_t color_green = lv_color_hex(0x058703);

// ---------------------------------------------------------------- 
// flip_couleur_panel_conso
// ---------------------------------------------------------------- 
void flip_couleur_panel_conso()
{
  static bool state;

  state = !state;
  
  if (state) {
    lv_obj_set_style_border_color(uic_panelconso, color_grey, LV_PART_MAIN);
  }
  else {
    switch (jour) {
      case JOUR_BLANC :
        lv_obj_set_style_border_color(uic_panelconso, color_white, LV_PART_MAIN);
        break;
      case JOUR_BLEU :
        lv_obj_set_style_border_color(uic_panelconso, color_blue, LV_PART_MAIN);
        break;
      case JOUR_ROUGE :
        lv_obj_set_style_border_color(uic_panelconso, color_red, LV_PART_MAIN);
        break;
      case JOUR_SANS :
        lv_obj_set_style_border_color(uic_panelconso, color_black, LV_PART_MAIN);
        break;
    }
  }
}

// ---------------------------------------------------------------- 
// set_couleur_jour
// ---------------------------------------------------------------- 
void set_couleur_jour(int couleur)
{
  String buffer;

  switch (couleur) {
    case JOUR_ROUGE:
      buffer = "Aujourd'hui : ROUGE";
      lv_obj_set_style_bg_color(ui_panelajourdhui, color_red, LV_PART_MAIN); 
      lv_obj_set_style_border_color(ui_panelajourdhui, color_white, LV_PART_MAIN);
      lv_obj_set_style_text_color(ui_aujourdhui, color_white, LV_PART_MAIN); 
      break;
    case JOUR_BLANC:
      buffer = "Aujourd'hui : BLANC";
      lv_obj_set_style_bg_color(ui_panelajourdhui, color_white, LV_PART_MAIN); 
      lv_obj_set_style_border_color(ui_panelajourdhui, color_grey, LV_PART_MAIN);
      lv_obj_set_style_text_color(ui_aujourdhui, color_grey, LV_PART_MAIN); 
      break;
    case JOUR_BLEU:
      buffer = "Aujourd'hui : BLEU";
      lv_obj_set_style_bg_color(ui_panelajourdhui, color_blue, LV_PART_MAIN); 
      lv_obj_set_style_border_color(ui_panelajourdhui, color_white, LV_PART_MAIN);
      lv_obj_set_style_text_color(ui_aujourdhui, color_white, LV_PART_MAIN); 
      break;
    default:
      buffer = "Aujourd'hui : sans couleur";
      lv_obj_set_style_bg_color(ui_panelajourdhui, color_grey, LV_PART_MAIN); 
      lv_obj_set_style_border_color(ui_panelajourdhui, color_white, LV_PART_MAIN);
      lv_obj_set_style_text_color(ui_aujourdhui, color_white, LV_PART_MAIN); 
      break;
  }

  lv_label_set_text(ui_aujourdhui, buffer.c_str()); 
}

// ---------------------------------------------------------------- 
// set_couleur_demain
// ---------------------------------------------------------------- 
void set_couleur_demain(int couleur)
{
  String buffer;

  switch (couleur) {
    case JOUR_ROUGE:
      buffer = "Demain : ROUGE";
      lv_obj_set_style_bg_color(ui_paneldemain, color_red, LV_PART_MAIN); 
      lv_obj_set_style_border_color(ui_paneldemain, color_white, LV_PART_MAIN);
      lv_obj_set_style_text_color(ui_demain, color_white, LV_PART_MAIN); 
      break;
    case JOUR_BLANC:
      buffer = "Demain : BLANC";
      lv_obj_set_style_bg_color(ui_paneldemain, color_white, LV_PART_MAIN); 
      lv_obj_set_style_border_color(ui_paneldemain, color_grey, LV_PART_MAIN);
      lv_obj_set_style_text_color(ui_demain, color_grey, LV_PART_MAIN); 
      break;
    case JOUR_BLEU:
      buffer = "Demain : BLEU";
      lv_obj_set_style_bg_color(ui_paneldemain, color_blue, LV_PART_MAIN); 
      lv_obj_set_style_border_color(ui_paneldemain, color_white, LV_PART_MAIN);
      lv_obj_set_style_text_color(ui_demain, color_white, LV_PART_MAIN); 
      break;
    default:
      buffer = "Demain : sans couleur";
      lv_obj_set_style_bg_color(ui_paneldemain, color_grey, LV_PART_MAIN); 
      lv_obj_set_style_border_color(ui_paneldemain, color_white, LV_PART_MAIN);
      lv_obj_set_style_text_color(ui_demain, color_white, LV_PART_MAIN); 
      break;
  }

  lv_label_set_text(ui_demain, buffer.c_str()); 
}

// ---------------------------------------------------------------- 
// verif_tarif
// ---------------------------------------------------------------- 
void verif_tarif(uint8_t label, char *valeur)
{

  if (flag_tic_standard == false) { // historique

    if (label == 0x02) { // OPTARIF
      if (strncmp(valeur, "BBR", 3) == 0) {
        tarif = TARIF_TEMPO;
        Serial.println(F("H TARIF_TEMPO"));
      }
      else {
        tarif = TARIF_AUTRE;
				Serial.println(F("H TARIF_AUTRE"));
      }
    }

    if (label == 0x10) { // PTEC
      if (tarif == TARIF_TEMPO) {
        jour=JOUR_SANS;
        heure=HEURE_TOUTE;

        if (strcmp(valeur, "HCJB") == 0) {
          jour=JOUR_BLEU;
          heure=HEURE_CREUSE;
          Serial.println(F("j bleu, h creuse"));
        }
        if (strcmp(valeur, "HCJW") == 0) {
          jour=JOUR_BLANC;
          heure=HEURE_CREUSE;
          Serial.println(F("j blanc, h creuse"));
        }
        if (strcmp(valeur, "HCJR") == 0) {
          jour=JOUR_ROUGE;
          heure=HEURE_CREUSE;
          Serial.println(F("j rouge, h creuse"));
        }
        if (strcmp(valeur, "HPJB") == 0) {
          jour=JOUR_BLEU;
          heure=HEURE_PLEINE;
          Serial.println(F("j bleu, h pleine"));
        }
        if (strcmp(valeur, "HPJW") == 0) {
          jour=JOUR_BLANC;
          heure=HEURE_PLEINE;
          Serial.println(F("j blanc, h pleine"));
        }
        if (strcmp(valeur, "HPJR") == 0) {
          jour=JOUR_ROUGE;
          heure=HEURE_PLEINE;
          Serial.println(F("j rouge, h pleine"));
        }
      }
    }

    if (label == 0x11) { // DEMAIN
      Serial.print(F("DEMAIN:"));
      Serial.println(valeur);
	    demain=JOUR_SANS;

  	  if (strcmp(valeur, "BLEU") == 0) {
    		demain=JOUR_BLEU;
    		Serial.println(F("bleu"));
  	  }
  	  if (strcmp(valeur, "BLAN") == 0) {
    		demain=JOUR_BLANC;
    		Serial.println(F("blanc"));
  	  }
  	  if (strcmp(valeur, "ROUG") == 0) {
    		demain=JOUR_ROUGE;
    		Serial.println(F("rouge"));
  	  }
	  }
  }

  set_couleur_jour(jour);
  set_couleur_demain(demain);

  switch (heure) {
    case HEURE_CREUSE:
      lv_label_set_text(ui_periode, "HC"); 
      lv_obj_set_style_text_color(ui_demain, color_green, LV_PART_MAIN); 
      break;
    case HEURE_PLEINE:
      lv_label_set_text(ui_periode, "HP"); 
      lv_obj_set_style_text_color(ui_demain, color_red, LV_PART_MAIN); 
      break;
    default :
      lv_label_set_text(ui_periode, ""); 
  }
}

// ---------------------------------------------------------------- 
// onReceive
// ---------------------------------------------------------------- 
void onReceive(int packetSize) {

  uint8_t buffer[sizeof(payload_data)]; 
  char buff_value[PAYLOAD_VALUE_LEN];
  bool flag = true;


  if (packetSize > 0) {
    Serial.print("onReceive:");
    Serial.println(packetSize);
  }

  if (packetSize == sizeof(payload_data)) {

    LoRa.readBytes(buffer, packetSize);

    if (is_valid_sender_id(buffer[0]) == true) {
      if (payload_deserialize(&payload_data, buffer, packetSize)) {
        
        flip_couleur_panel_conso();
        if (flag_associated == true) {
          payload_get_value_str(&payload_data, buff_value);

          Serial.print(payload_data.label_id);
          Serial.print(" : ");
          Serial.println(buff_value);
          if (payload_data.sender_id == LORALINK_S) flag_tic_standard = true;
            else flag_tic_standard = false;

          if (payload_data.label_id == 0x1A || payload_data.label_id == 0x22) { // PAPP/SINSTS
            lv_label_set_text(ui_conso, String(String(atol(buff_value)) + " W").c_str()); 
            if (puissance_max > 0) {
              lv_bar_set_value(ui_barconso,puissance_max/atol(buff_value),LV_ANIM_OFF);
            }
          }
          if (payload_data.label_id == 0x03) { // ISOUSC
            puissance_max = atol(buff_value) * 230;
          }
          if (payload_data.label_id == 0x21) { // PCOUP
            puissance_max = atol(buff_value);
          }
          verif_tarif(payload_data.label_id, buff_value);
        }
		else {
		  if (payload_data.label_id == 0xFF) { // Association
          flag_associated = true;
          cpt_id = payload_get_counter_addr_toUint64(&payload_data);
          preferences.begin("fbs", false);
          preferences.putULong64("CPTID", cpt_id);
          preferences.end();
          String buff = "Cpt " + String(cpt_id) + " associé";
          Serial.println(cpt_id);
          lv_label_set_text(ui_conso, buff.c_str()); 
          lv_handler(); 
        }
		}
      }
      else {
        lv_obj_set_style_border_color(uic_panelconso, color_red, LV_PART_MAIN);;
        Serial.println(F("❌ CRC invalide"));
        flag = false;
        lv_obj_set_style_border_color(uic_panelconso, color_white, LV_PART_MAIN);
      }
    }
    else {
      lv_obj_set_style_border_color(uic_panelconso, color_purple, LV_PART_MAIN);
      Serial.println(F("❌ Entete invalide"));
      flag = false;
      lv_obj_set_style_border_color(uic_panelconso, color_white, LV_PART_MAIN);
    }
  }
}


void setup()
{
  pinMode(bpPin, INPUT);
  lv_begin();  // Initialize lvgl with display and touch
  delay(2000);

  Serial.println(F(" ███████╗██████╗ ███████╗"));
  Serial.println(F(" ██╔════╝██╔══██╗██╔════╝"));
  Serial.println(F(" █████╗  ██████╔╝███████╗"));
  Serial.println(F(" ██╔══╝  ██╔══██╗╚════██║"));
  Serial.println(F(" ██║     ██████╔╝███████║"));
  Serial.println(F(" ╚═╝     ╚═════╝ ╚══════╝"));
  Serial.print(F(" TempoView  v"));
  Serial.println(VERSION);

  ui_init();   // Initialize UI generated by Square Line
  
  mySPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setSPI(mySPI);

  // Demarrage module Lora ------
  Serial.print(F("Init LoRa : "));
  LoRa.setPins(SS, RST, DIO0);
  if (LoRa.begin(frequency)) {
    LoRa.setSpreadingFactor(LORA_SF);
    LoRa.setSignalBandwidth(LORA_BW);
    LoRa.setCodingRate4(LORA_CR);
    LoRa.setPreambleLength(LORA_PREAMBLE_LENGTH);
    LoRa.enableCrc();
    LoRa.setTxPower(LORA_POWER);
	
	  Serial.println(F("ok"));
  }
  else {
    Serial.println(F("ko!"));
	  lv_obj_set_style_text_color(ui_conso, color_red, LV_PART_MAIN); 
    lv_label_set_text(ui_conso, "<< LoRa init KO ! >>");
    lv_handler(); 
    delay(2000);
  }
  
  preferences.begin("fbs", false);
  if (digitalRead(bpPin) == LOW) { // reset preferences
    Serial.println(F("<< Reset association >>"));
    lv_obj_set_style_text_color(ui_conso, color_red, LV_PART_MAIN); 
    lv_label_set_text(ui_conso, "<< Reset association >>"); 
    lv_handler(); 
    preferences.putULong64("CPTID", 0);
    cpt_id=0;
    flag_associated=false;
    delay(1000);
    lv_label_set_text(ui_conso, "<< non associe >>"); 
    lv_handler(); 
  }
  else {
    Serial.print(F("Verif association:"));
    cpt_id = preferences.getULong64("CPTID");
    if (cpt_id == 0) {
      flag_associated=false;
      Serial.println(F("KO"));
	    lv_obj_set_style_text_color(ui_conso, color_red, LV_PART_MAIN); 
	    lv_label_set_text(ui_conso, "<< non associe >>"); 
      lv_handler(); 
    }
    else {
      flag_associated=true;
	    lv_obj_set_style_text_color(ui_conso, color_black, LV_PART_MAIN); 
	    lv_label_set_text(ui_conso, String(cpt_id).c_str()); 
      lv_handler(); 
      Serial.print(F("OK : "));
      Serial.println(cpt_id);
    }
  }
  preferences.end();
}



void loop()
{
  lv_handler();  // Update UI

  onReceive(LoRa.parsePacket());
}