//------------------------------------------------------------------------------
//! Contains declarations of the following hash functions:
//! 1. TODO:
//! 
//! @file   hash_functions.h
//! @author tralf-strues
//! @date   2021-04-02
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#pragma once

#include <stdint.h>

uint32_t getConstantHash       (const char* string);
uint32_t getStrLenHash         (const char* string);
uint32_t getFirstCharHash      (const char* string);
uint32_t getSumCharsHash       (const char* string);
uint32_t getXorRightRotateHash (const char* string);
uint32_t getXorLeftRotateHash  (const char* string);
// uint32_t getMurmur3Hash        (const char* string);
extern uint32_t getMurmur3Hash (const char* string);