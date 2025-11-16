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
#include "Label_tic.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


LabelMap tic_standard_labels[] = {
    { "ADSC", 0x01, "", "", 12 },
    { "VTIC", 0x02, "", "", 2 },
    { "DATE", 0x03, "", "date", 0 },
    { "NGTF", 0x04, "", "", 16 },
    { "LTARF", 0x05, "", "", 16 },
    { "EAST", 0x06, "Wh", "energy", 9 },
    { "EASF01", 0x07, "Wh", "energy", 9 },
    { "EASF02", 0x08, "Wh", "energy", 9 },
    { "EASF03", 0x09, "Wh", "energy", 9 },
    { "EASF04", 0x0A, "Wh", "energy", 9 },
    { "EASF05", 0x0B, "Wh", "energy", 9 },
    { "EASF06", 0x0C, "Wh", "energy", 9 },
    { "EASF07", 0x0D, "Wh", "energy", 9 },
    { "EASF08", 0x0E, "Wh", "energy", 9 },
    { "EASF09", 0x0F, "Wh", "energy", 9 },
    { "EASF10", 0x10, "Wh", "energy", 9 },
    { "EASD01", 0x11, "Wh", "energy", 9 },
    { "EASD02", 0x12, "Wh", "energy", 9 },
    { "EASD03", 0x13, "Wh", "energy", 9 },
    { "EASD04", 0x14, "Wh", "energy", 9 },
    { "EAIT", 0x15, "Wh", "energy", 9 },
    { "ERQ1", 0x16, "VArh", "energy", 9 },
    { "ERQ2", 0x17, "VArh", "energy", 9 },
    { "ERQ3", 0x18, "VArh", "energy", 9 },
    { "ERQ4", 0x19, "VArh", "energy", 9 },
    { "IRMS1", 0x1A, "A", "current", 3 },
    { "IRMS2", 0x1B, "A", "current", 3 },
    { "IRMS3", 0x1C, "A", "current", 3 },
    { "URMS1", 0x1D, "V", "voltage", 3 },
    { "URMS2", 0x1E, "V", "voltage", 3 },
    { "URMS3", 0x1F, "V", "voltage", 3 },
    { "PREF", 0x20, "kVA", "apparent_power", 2 },
    { "PCOUP", 0x21, "kVA", "apparent_power", 2 },
    { "SINSTS", 0x22, "VA", "apparent_power", 5 },
    { "SINSTS1", 0x23, "VA", "apparent_power", 5 },
    { "SINSTS2", 0x24, "VA", "apparent_power", 5 },
    { "SINSTS3", 0x25, "VA", "apparent_power", 5 },
    { "SMAXSN", 0x26, "VA", "apparent_power", 5 },
    { "SMAXSN1", 0x27, "VA", "apparent_power", 5 },
    { "SMAXSN2", 0x28, "VA", "apparent_power", 5 },
    { "SMAXSN3", 0x29, "VA", "apparent_power", 5 },
    { "SMAXSN-1", 0x2A, "VA", "apparent_power", 5 },
    { "SMAXSN1-1", 0x2B, "VA", "apparent_power", 5 },
    { "SMAXSN2-1", 0x2C, "VA", "apparent_power", 5 },
    { "SMAXSN3-1", 0x2D, "VA", "apparent_power", 5 },
    { "SINSTI", 0x2E, "VA", "apparent_power", 5 },
    { "SMAXIN", 0x2F, "VA", "apparent_power", 5 },
    { "SMAXIN-1", 0x30, "VA", "apparent_power", 5 },
    { "CCASN", 0x31, "W", "energy", 5 },
    { "CCASN-1", 0x32, "W", "energy", 5 },
    { "CCAIN", 0x33, "W", "energy", 5 },
    { "CCAIN-1", 0x34, "W", "energy", 5 },
    { "UMOY1", 0x35, "V", "voltage", 3 },
    { "UMOY2", 0x36, "V", "voltage", 3 },
    { "UMOY3", 0x37, "V", "voltage", 3 },
    { "STGE", 0x38, "", "", 8 },
    { "DPM1", 0x39, "", "", 2 },
    { "FPM1", 0x3A, "", "", 2 },
    { "DPM2", 0x3B, "", "", 2 },
    { "FPM2", 0x3C, "", "", 2 },
    { "DPM3", 0x3D, "", "", 2 },
    { "FPM3", 0x3E, "", "", 2 },
    { "MSG1", 0x3F, "", "", 32 },
    { "MSG2", 0x40, "", "", 16 },
    { "PMR", 0x41, "", "", 14 },
    { "RELAIS", 0x42, "", "", 3 },
    { "NTARF", 0x43, "", "", 2 },
    { "NJOURF", 0x44, "", "", 2 },
    { "NJOURF+1", 0x45, "", "", 2 },
    { "PJOURF+1", 0x46, "", "", 98 },
    { "PPOINTE", 0x47, "", "", 98 },
    { "BOOT", 0xFE, "", "", 0 },
    { "VERSION", 0xFF, "", "", 0 }
};
const size_t tic_standard_count = sizeof(tic_standard_labels) / sizeof(LabelMap);

LabelMap tic_historical_labels[] = {
    { "ADCO", 0x01, "", "", 12 },
    { "OPTARIF", 0x02, "", "", 4 },
    { "ISOUSC", 0x03, "A", "current", 2 },
    { "BASE", 0x04, "Wh", "energy", 9 },
    { "HCHC", 0x05, "Wh", "energy", 9 },
    { "HCHP", 0x06, "Wh", "energy", 9 },
    { "EJPHN", 0x07, "Wh", "energy", 9 },
    { "EJPHPM", 0x08, "Wh", "energy", 9 },
    { "BBRHCJB", 0x09, "Wh", "energy", 9 },
    { "BBRHPJB", 0x0A, "Wh", "energy", 9 },
    { "BBRHCJW", 0x0B, "Wh", "energy", 9 },
    { "BBRHPJW", 0x0C, "Wh", "energy", 9 },
    { "BBRHCJR", 0x0D, "Wh", "energy", 9 },
    { "BBRHPJR", 0x0E, "Wh", "energy", 9 },
    { "PEJP", 0x0F, "mn", "duration", 2 },
    { "PTEC", 0x10, "", "", 4 },
    { "DEMAIN", 0x11, "", "", 4 },
    { "IINST", 0x12, "A", "current", 3 },
    { "IINST1", 0x13, "A", "current", 3 },
    { "IINST2", 0x14, "A", "current", 3 },
    { "IINST3", 0x15, "A", "current", 3 },
    { "IMAX", 0x16, "A", "current", 3 },
    { "IMAX1", 0x17, "A", "current", 3 },
    { "IMAX2", 0x18, "A", "current", 3 },
    { "IMAX3", 0x19, "A", "current", 3 },
    { "PAPP", 0x1A, "VA", "apparent_power", 5 },
    { "PMAX", 0x1B, "VA", "apparent_power", 5 },
    { "ADPS", 0x1C, "A", "current", 3 },
    { "HHPHC", 0x1D, "", "", 1 },
    { "MOTDETAT", 0x1E, "", "", 6 },
    { "PPOT", 0x1F, "", "", 2 },
    { "BOOT", 0xFE, "", "", 0 },
    { "VERSION", 0xFF, "", "", 0 }
};
const size_t tic_historical_count = sizeof(tic_historical_labels) / sizeof(LabelMap);

// Trouve l’ID d’un label
uint8_t find_label_id(const char* label, bool is_standard) {
    LabelMap* map = is_standard ? tic_standard_labels : tic_historical_labels;
    size_t count = is_standard ? tic_standard_count : tic_historical_count;

    for (size_t i = 0; i < count; i++) {
        if (strcmp(map[i].label, label) == 0) {
            return map[i].id;
        }
    }
    return 0; // 0 représente un ID invalide
}

uint8_t get_label_length(const char* label, bool is_standard) {
    LabelMap* map = is_standard ? tic_standard_labels : tic_historical_labels;
    size_t count = is_standard ? tic_standard_count : tic_historical_count;

    for (size_t i = 0; i < count; i++) {
        if (strcmp(map[i].label, label) == 0) {
            return map[i].len;
        }
    }
    return 0; // 0 si non trouvé
}

// Retourne le nom d’étiquette à partir de son ID
const char* find_label_str(uint8_t id, bool is_standard) {
    LabelMap* map = is_standard ? tic_standard_labels : tic_historical_labels;
    size_t count = is_standard ? tic_standard_count : tic_historical_count;

    for (size_t i = 0; i < count; i++) {
        if (map[i].id == id) {
            return map[i].label;
        }
    }
    return NULL; // NULL si non trouvé
}

const LabelMap* find_label_by_id(uint8_t id, bool is_standard) {
    LabelMap* map = is_standard ? tic_standard_labels : tic_historical_labels;
    size_t count = is_standard ? tic_standard_count : tic_historical_count;
    
    for (size_t i = 0; i < count; i++) {
        if (map[i].id == id) {
            return &map[i];
        }
    }
    return NULL;  // NULL si non trouvé
}

uint8_t get_label_length_by_id(uint8_t id, bool is_standard) {
    const LabelMap* label = find_label_by_id(id, is_standard);
    if (label) {
        return label->len;
    }
    return 0;  // 0 si non trouvé
}

bool is_energy_related(uint8_t id, bool is_standard) {
    const LabelMap *label = find_label_by_id(id, is_standard);
    if (!label || !label->device_class) {
        return false;
    }

    return (
        strcmp(label->device_class, "energy") == 0 ||
        strcmp(label->device_class, "current") == 0 ||
        strcmp(label->device_class, "voltage") == 0 ||
        strcmp(label->device_class, "apparent_power") == 0
    );
}

