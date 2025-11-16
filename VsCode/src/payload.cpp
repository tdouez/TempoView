//--------------------------------------------------------------------
// ███████╗██████╗ ███████╗
// ██╔════╝██╔══██╗██╔════╝
// █████╗  ██████╔╝███████╗
// ██╔══╝  ██╔══██╗╚════██║
// ██║     ██████╔╝███████║
// ╚═╝     ╚═════╝ ╚══════╝
// FuméeBleueSolutions
//--------------------------------------------------------------------
// Code pour le module LoraLink de la société FBS. 
//--------------------------------------------------------------------
// Copyright (c) 2025 - FBS
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to use,
// copy, modify, and redistribute the Software for non-commercial purposes only,
// provided that the following conditions are met:
//
// 1. Attribution:
//    Any use, copy, modification, or redistribution must explicitly credit
//    the original author: "Tristan".
//
// 2. Redistribution:
//    Any redistribution of this Software or derivatives must retain this
//    license text, including the attribution requirement and the
//    non-commercial clause.
//
// 3. Non-Commercial Use:
//    Commercial use of the Software or any derivatives is strictly prohibited
//    without prior written permission from the author.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//--------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h> 
#include <stdio.h>
#include "payload.h"
#include <Arduino.h>
#include <string.h>
#include "xtea.h"


uint64_t str_to_u64(const char *str) {
    uint64_t val = 0;

    if (str != NULL) {
        while (*str >= '0' && *str <= '9') {
            val = val * 10 + (*str - '0');
            str++;
        }
    }
    return val;
}

const char* sender_id_to_string(uint8_t id) {
    switch (id) {
        case LORALINK_H: return "Linky historique";
        case LORALINK_S: return "Linky standard";
        case LORAGAZ:    return "Compteur Gaz";
        case TEMPOLINK:  return "TempoLink";
        case TEMPOVIEW:  return "TempoView";
        default:         return "Inconnu";
    }
}

void payload_set_value_str(PayloadData* p, const char* str) {
    if (p != NULL && str != NULL) {
        strncpy(p->value, str, PAYLOAD_VALUE_LEN - 1);
        p->value[PAYLOAD_VALUE_LEN - 1] = '\0';
    }
}

void payload_set_counter_addr(PayloadData* p, const char* str)  {
    if (p != NULL && str != NULL) {
        //Serial.print("payload_set_counter_addr:");
        //Serial.println(str);
        uint64_t val = str_to_u64(str);
        p->counter_addr[0] = (val >> 32) & 0xFF;
        p->counter_addr[1] = (val >> 24) & 0xFF;
        p->counter_addr[2] = (val >> 16) & 0xFF;
        p->counter_addr[3] = (val >> 8)  & 0xFF;
        p->counter_addr[4] = val & 0xFF;
    }
}

void payload_get_counter_addr_toString(const PayloadData* p, char* out, size_t max_len) {
    if (!p || !out || max_len == 0) return;

    uint64_t val = payload_get_counter_addr_toUint64(p);
#if defined(ARDUINO_ARCH_AVR)
    // AVR ne gère pas %llu → on reconvertit manuellement
    char buf[21];
    char *ptr = &buf[20];
    *ptr = '\0';
    if (val == 0) *--ptr = '0';
    while (val > 0) {
        *--ptr = '0' + (val % 10);
        val /= 10;
    }
    strncpy(out, ptr, max_len - 1);
    out[max_len - 1] = '\0';
#else
    snprintf(out, max_len, "%llu", val);
#endif
}

uint64_t payload_get_counter_addr_toUint64(const PayloadData* p) {
    uint64_t val = 0;

    if (p != NULL) {
        val =  ((uint64_t)p->counter_addr[0] << 32) |
                ((uint64_t)p->counter_addr[1] << 24) |
                ((uint64_t)p->counter_addr[2] << 16) |
                ((uint64_t)p->counter_addr[3] << 8)  |
                ((uint64_t)p->counter_addr[4]);
    }
    return val;
}

void payload_get_value_str(const PayloadData* p, char* out) {
    if (p != NULL && out != NULL) {
		strncpy(out, p->value, PAYLOAD_VALUE_LEN);
		out[PAYLOAD_VALUE_LEN-1] = '\0';
    }
}

// CRC8 simple (polynôme x^8 + x^2 + x + 1 = 0x07)
uint8_t payload_compute_crc(const PayloadData* p) {
    uint8_t crc = 0;

    if (p != NULL) {
        // Sender ID
        crc ^= p->sender_id;
        for (uint8_t j = 0; j < 8; j++)
            crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1);

        // Adresse compteur 
        for (uint8_t i = 0; i < 5; i++) {
            crc ^= p->counter_addr[i];
            for (int j = 0; j < 8; j++)
                crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1);
        }

        // Label ID
        crc ^= p->label_id;
        for (uint8_t j = 0; j < 8; j++)
            crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1);

        // Valeur 
        for (int i = 0; i < PAYLOAD_VALUE_LEN; i++) {
            crc ^= p->value[i];
            for (uint8_t j = 0; j < 8; j++)
                crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1);
        }

        // batt_level
        crc ^= p->batt_level;
        for (uint8_t j = 0; j < 8; j++)
            crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1);
        
    }

    //Serial.print("payload_compute_crc:");
    //Serial.println(crc, HEX);
    return crc;
}

bool payload_check_crc(const PayloadData* p) {
    if (p != NULL) return payload_compute_crc(p) == p->crc;
        else return false;
}

void payload_finalize(PayloadData* p) {
    if (p != NULL) {
        p->crc = payload_compute_crc(p);

    }
}

//------------------------------------------------------------------
// Ecriture explicite pour éviter des éventuels problèmes d'alignements sur les procs AVR/ESP32
int payload_serialize(const PayloadData* p, uint8_t* buffer) {
    if (!p || !buffer) return 0;

    uint8_t* ptr = buffer;

    *ptr++ = p->sender_id;

    memcpy(ptr, p->counter_addr, COUNTER_ADDR_LEN); 
    ptr += 5;

    *ptr++ = p->label_id;

    memcpy(ptr, p->value, PAYLOAD_VALUE_LEN);
    ptr += PAYLOAD_VALUE_LEN;

    *ptr++ = p->batt_level;

    *ptr++ = p->crc;

    return ptr - buffer; // longueur totale
}

//------------------------------------------------------------------
// Ecriture explicite pour éviter des éventuels problèmes d'alignements sur les procs AVR/ESP32
bool payload_deserialize(PayloadData* p, const uint8_t* buffer, int length) {
    if (!p || !buffer) return false;

    const uint8_t* ptr = buffer;

    if (length < (1 + COUNTER_ADDR_LEN + 1 + PAYLOAD_VALUE_LEN + 1 + 1)) return false;

    p->sender_id = *ptr++;
    memcpy(p->counter_addr, ptr, COUNTER_ADDR_LEN);
    ptr += COUNTER_ADDR_LEN;
    p->label_id = *ptr++;
    memcpy(p->value, ptr, PAYLOAD_VALUE_LEN);
    ptr += PAYLOAD_VALUE_LEN;
    p->batt_level = *ptr++;
    p->crc = *ptr++;

    return payload_check_crc(p);
}

static void encrypt_payload(uint8_t* data, size_t len) {
    uint8_t block[8];
    for (size_t i = 0; i + 8 <= len; i += 8) {
        memcpy(block, &data[i], 8);
        xtea_encrypt((uint32_t*)block, xtea_key);
        memcpy(&data[i], block, 8);
    }
}

void printUint64(uint64_t value) {
  char buffer[21]; // Assez pour 20 chiffres + '\0'
  char* ptr = &buffer[20];
  *ptr = '\0';

  if (value == 0) {
    Serial.print("0");
    return;
  }

  while (value > 0) {
    --ptr;
    *ptr = '0' + (value % 10);
    value /= 10;
  }

  Serial.print(ptr);
}

bool is_valid_sender_id(uint8_t id) {
  return (id >= LORALINK_H && id <= TEMPOVIEW);
}