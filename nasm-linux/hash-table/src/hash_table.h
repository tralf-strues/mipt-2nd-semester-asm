

#pragma once

#include <stdint.h>

#include <language.h>
#include <bucket.h>

typedef const char* key_t;

struct HashTable
{
    size_t   size;
    Bucket*  buckets;
    int      (*cmp)     (const value_t* first, const value_t* second);
    uint32_t (*getHash) (key_t key);
};

void     construct        (HashTable* hash_table, size_t size, 
                           int      (*cmp)     (const value_t* first, const value_t* second),
                           uint32_t (*getHash) (key_t key));

void     destroy          (HashTable* hash_table);
void     insert           (HashTable* hash_table, key_t key, value_t value);
void     reallocHashTable (HashTable* hash_table);
void     remove           (HashTable* hash_table, key_t key);
value_t* get              (const HashTable* hash_table, key_t key);