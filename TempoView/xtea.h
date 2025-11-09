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
#pragma once
#include <stdint.h>

static const uint32_t xtea_key[4] = {
    0x64697053, 0x6F437265, 0x6E6F6863, 0x53424620
};

void xtea_encrypt(uint32_t* v, const uint32_t key[4]);
void xtea_decrypt(uint32_t* v, const uint32_t key[4]);