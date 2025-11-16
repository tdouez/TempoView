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
#pragma once
#include <stdint.h>

static const uint32_t xtea_key[4] = {
    0x64697053, 0x6F437265, 0x6E6F6863, 0x53424620
};

void xtea_encrypt(uint32_t* v, const uint32_t key[4]);
void xtea_decrypt(uint32_t* v, const uint32_t key[4]);