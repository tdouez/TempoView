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
// Copyright (C) 2025 FBS. Tous droits réservés.
//
//    Ce programme ne peut être modifié ou redistribué sans 
//     l'accord explicite de la société FBS.
//
//--------------------------------------------------------------------
#ifndef Label_tic_h
#define Label_tic_h

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    const char* label;
    uint8_t id;
    const char* unite;
    const char* device_class;
} LabelMap;


uint8_t find_label_id(const char* label, bool is_standard);
const char* find_label_str(uint8_t id, bool is_standard);
const LabelMap* find_label_by_id(uint8_t id, bool is_standard);
bool is_energy_related(uint8_t id, bool is_standard);

#endif