#include "hash_table.h"

void construct(HashTable* hashTable, size_t size, 
               int (*cmp)(const ht_key_t first, const ht_key_t second),
               uint32_t (*getHash)(ht_key_t key))
{
    assert(hashTable);
    assert(size > 0);
    assert(cmp);
    assert(getHash);

    hashTable->buckets = (Bucket*) calloc(size, sizeof(Bucket));
    assert(hashTable->buckets);

    for (uint32_t i = 0; i < size; i++)
    {
        if (construct(hashTable->buckets + i) == nullptr)
        {
            assert(!"bucket construct failed");
        }
    }

    hashTable->size    = size;
    hashTable->cmp     = cmp;
    hashTable->getHash = getHash;
}

void destroy(HashTable* hashTable)
{
    assert(hashTable);

    if (hashTable->buckets != nullptr)
    {
        for (uint32_t i = 0; i < hashTable->size; i++)
        {
            destroy(hashTable->buckets + i);
        }

        free(hashTable->buckets);
    }

    hashTable->size    = 0;
    hashTable->buckets = nullptr;
    hashTable->cmp     = nullptr;
    hashTable->getHash = nullptr;
}

#ifdef AVX_STRING_OPTIMIZATION
__m256i strToYMM(const char* string)
{
    assert(string);

    char buffer[sizeof(__m256i)] = "";
    strcpy(buffer, string);
    
    return _mm256_loadu_si256((const __m256i_u*) buffer);
}
#endif

void insert(HashTable* hashTable, ht_key_t key, ht_value_t value)
{
    assert(hashTable);
    assert(hashTable->buckets);
    assert(hashTable->cmp);
    assert(hashTable->getHash);

    uint32_t hash   = (hashTable->getHash(key)) % hashTable->size;
    Bucket*  bucket = hashTable->buckets + hash; 

#ifdef AVX_STRING_OPTIMIZATION
    
    __m256i keyString = strToYMM(key);

    // Checks whether the value is already in bucket
    for (uint32_t i = 0; i < bucket->size; i++)
    {
        __m256i bucketElemString = bucket->data[i].key;
        __m256i cmpResult        = _mm256_cmpeq_epi8(keyString, bucketElemString);
        if (~_mm256_movemask_epi8(cmpResult) == 0)
        {
            return;
        }
    }

    pushBack(bucket, {keyString, value});

#else 
    
    // Checks whether the value is already in bucket
    for (uint32_t i = 0; i < bucket->size; i++)
    {
        if (hashTable->cmp(key, bucket->data[i].key) == 0)
        {
            return;
        }
    }    
    
    pushBack(bucket, {key, value});

#endif
}

#ifndef OPTIMIZED_FIND
const ht_value_t* find(const HashTable* hashTable, ht_key_t key)
{
    assert(hashTable);
    assert(hashTable->buckets);
    assert(hashTable->cmp);
    assert(hashTable->getHash);

    uint32_t hash   = hashTable->getHash(key) % hashTable->size;
    Bucket*  bucket = hashTable->buckets + hash; 
    
#ifdef AVX_STRING_OPTIMIZATION
    __m256i keyString = strToYMM(key);

    // Checks whether the value is already in bucket
    for (uint32_t i = 0; i < bucket->size; i++)
    {
        __m256i bucketElemString = bucket->data[i].key;
        __m256i cmpResult        = _mm256_cmpeq_epi8(keyString, bucketElemString);
        if (~_mm256_movemask_epi8(cmpResult) == 0)
        {
            return &(bucket->data[i].value);
        }
    }

    return nullptr;
#else 
    
    // Checks whether the value is already in bucket
    for (uint32_t i = 0; i < bucket->size; i++)
    {
        if (hashTable->cmp(key, bucket->data[i].key) == 0)
        {
            return &(bucket->data[i].value);
        }
    }    
    
    return nullptr;
#endif

}
#endif