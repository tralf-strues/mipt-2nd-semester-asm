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
#include <stdint.h>
#include "hash_table.h"

const size_t HASH_TABLE_SIZE = 127579; // load factor = 0.95

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
//! @param dictionary Dictionary to be initialized.
//! 
//! @note  Function loadDictionary() is supposed to be called after this 
//!        function.
//------------------------------------------------------------------------------
void initDictionary(Dictionary* dictionary);

#ifdef DEBUG
//------------------------------------------------------------------------------
//! Initializes the dictionary. Should be called prior to any usage of it.
//!
//! @param dictionary Dictionary to be initialized.
//! @param getHash    Specifies what hash function to use for the hash table 
//!                   (used for figuring out the optimal hash function). 
//!
//! @note  Function loadDictionary() is supposed to be called after this 
//!        function.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//! Initializes the dictionary. Should be called prior to any usage of it.
//! 
//! @param dictionary    Dictionary to be initialized.
//! @param hashTableSize Specifies the hash table size.
//! @param getHash       Specifies what hash function to use for the hash table 
//!                      (used for figuring out the optimal hash function). 
//! 
//! @note  Function loadDictionary() is supposed to be called after this 
//!        function.
//------------------------------------------------------------------------------
void initDictionary(Dictionary* dictionary, size_t hashTableSize, uint32_t (*getHash) (const char* value));
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
//! @param dictionary Initialized dictionary to which to load the parsed data.
//! @param buffer     Buffer containing the text of the supported database.
//! @param bufferSize Size of the buffer in bytes.
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

//------------------------------------------------------------------------------
//! Writes a full dictionary entry to output file. Example of a printed text 
//! (for the word 'bark'):
//! 1. [v.t.] To strip the bark from; to peel.
//! 2. [v.t.] To abrade or rub off any outer covering from; as to bark one's 
//! heel.
//! 3. [v.t.] To girdle. See Girdle, v. t., 3.
//! 4. [v.t.] To cover or inclose with bark, or as with bark; as, to bark the 
//! roof of a hut.
//! 5. [v.i.] To make a short, loud, explosive noise with the vocal organs; -- 
//! said of some animals, but especially of dogs.
//! 6. [v.i.] To make a clamor; to make importunate outcries.
//! 7. [n] The short, loud, explosive sound uttered by a dog; a similar sound 
//! made by some other animals.
//! 8. [n] Alt. of Barque
//! 
//! @param output
//! @param dictEntry
//------------------------------------------------------------------------------
void printDictEntry(FILE* output, const DictEntry* dictEntry);