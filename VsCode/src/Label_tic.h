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
#ifndef Label_tic_h
#define Label_tic_h

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    const char* label;
    uint8_t id;
    const char* unite;
    const char* device_class;
    uint8_t len;
} LabelMap;


uint8_t find_label_id(const char* label, bool is_standard);
const char* find_label_str(uint8_t id, bool is_standard);
const LabelMap* find_label_by_id(uint8_t id, bool is_standard);
uint8_t get_label_length_by_id(uint8_t id, bool is_standard);
uint8_t get_label_length(const char* label, bool is_standard);
bool is_energy_related(uint8_t id, bool is_standard);

#endif