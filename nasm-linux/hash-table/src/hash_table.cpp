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
            assert(!"bucket construct failed"); // TODO: 
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

// TODO: DEBUG
size_t heres = 0;

void insert(HashTable* hashTable, ht_key_t key, ht_value_t value)
{
    assert(hashTable);
    assert(hashTable->buckets);
    assert(hashTable->cmp);
    assert(hashTable->getHash);

    uint32_t hash   = (hashTable->getHash(key)) % hashTable->size;
    Bucket*  bucket = hashTable->buckets + hash; 

    // Checks whether the value is already in bucket
    for (uint32_t i = 0; i < bucket->size; i++)
    {
        if (hashTable->cmp(key, bucket->data[i].key) == 0)
        {
            return;
        }
    }

    pushBack(bucket, {key, value});
}

void reallocHashTable(HashTable* hashTable)
{
    assert(hashTable);
    // TODO:
}

void remove(HashTable* hashTable, ht_key_t key)
{
    assert(hashTable);
    assert(hashTable->buckets);
    // TODO:
}

const ht_value_t* get(const HashTable* hashTable, ht_key_t key)
{
    assert(hashTable);
    assert(hashTable->buckets);
    assert(hashTable->cmp);
    assert(hashTable->getHash);

    uint32_t hash   = hashTable->getHash(key) % hashTable->size;
    Bucket*  bucket = hashTable->buckets + hash; 
    
    int (*cmp) (const ht_key_t first, const ht_key_t second) = hashTable->cmp;
    for (uint32_t i = 0; i < bucket->size; i++)
    {
        // Checks whether the value is already in hashTable
        if (cmp(key, bucket->data[i].key) == 0)
        {
            return &(bucket->data[i].value);
        }
    }

    return nullptr;
}