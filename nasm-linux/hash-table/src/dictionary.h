//------------------------------------------------------------------------------
//! Declares Dictionary struct and functions related to it.
//! 
//! @file   dictionary.h
//! @author tralf-strues
//! @date   2021-04-01
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#pragma once
 
#include <stdio.h>
#include "hash_table.h"

const size_t HASH_TABLE_SIZE = 100003;

struct Dictionary
{
    HashTable   hashTable;
    size_t      wordsCount;
    size_t      definitionsCount;
    Definition* definitions;
};

//------------------------------------------------------------------------------
//! Initializes the dictionary. Should be called prior to any usage of it.
//!
//! @param dictionary - Dictionary to be initialized.
//! 
//! @note  Function loadDictionary is supposed to be called after this function.
//------------------------------------------------------------------------------
void initDictionary(Dictionary* dictionary);

#ifdef DEBUG
//------------------------------------------------------------------------------
//! Initializes the dictionary. Should be called prior to any usage of it.
//!
//! @param dictionary - Dictionary to be initialized.
//! @param getHash    - Specifies what hash function to use for the hash table 
//!                     (used for figuring out the optimal hash function). 
//!
//! @note  Function loadDictionary is supposed to be called after this function.
//------------------------------------------------------------------------------
void initDictionary(Dictionary* dictionary, uint64_t (*getHash)(key_t key));
#endif

//------------------------------------------------------------------------------
//! Frees used memory and resets dictionary's values.
//!
//! @param dictionary 
//------------------------------------------------------------------------------
void destroyDictionary(Dictionary* dictionary);

//------------------------------------------------------------------------------
//! Loads a dictionary from a buffer containing the text of the supported 
//! database.
//! 
//! @param dictionary - Initialized dictionary to which to load the parsed data.
//! @param buffer     - Buffer containing the text of the supported database.
//! @param bufferSize - Size of the buffer in bytes.
//!
//! @warning The dictionary has to be initialized (see initDictionary)!
//------------------------------------------------------------------------------
void loadDictionary(Dictionary* dictionary, char* buffer, size_t bufferSize);

//------------------------------------------------------------------------------
//! Finds the dictionary entry for the word.
//! 
//! @param dictionary
//! @param word      
//! 
//! @return nullptr if the dictionary doesn't contain any word's defintions, or 
//!         a dictionary entry for the word otherwise.
//------------------------------------------------------------------------------
const DictEntry* getDictionaryEntry(const Dictionary* dictionary, const char* word);