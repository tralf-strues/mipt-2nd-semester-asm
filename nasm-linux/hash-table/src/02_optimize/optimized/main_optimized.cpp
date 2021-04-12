#include <time.h>
#include "../../file_loader.h"
#include "../../hash_table.h"
#include "../../hash_functions.h"

const size_t WORDS_COUNT      = 466471; 
const size_t HASH_TABLE_SIZE  = 491039; // 466,472 / 0.95 = 491,023 or 491039 (the nearest prime) 
const size_t INSERTIONS_COUNT = 5;
const size_t SEARCHES_COUNT   = 200; 

const char** readWords      (char** buffer);
void         testInsertions (HashTable* hashTable, const char** words); 
void         testSearches   (HashTable* hashTable, const char** words);

int main()
{
    timespec startTime = {};
    clock_gettime(CLOCK_MONOTONIC, &startTime);

    char*        buffer = nullptr;
    const char** words  = readWords(&buffer);
    assert(buffer);
    assert(words);

    HashTable hashTable = {};

    #ifdef HASH_TABLE_ONLY
    construct(&hashTable, HASH_TABLE_SIZE, strcmp, getCrc32Hash); 
    #endif
    #ifdef CRC32_OPTIMIZED
    construct(&hashTable, HASH_TABLE_SIZE, strcmp, getOPCrc32Hash); 
    #endif

    for (uint32_t i = 0; i < INSERTIONS_COUNT; i++)
    {
        testInsertions(&hashTable, words);
    }

    for (uint32_t i = 0; i < SEARCHES_COUNT; i++)
    {
        testSearches(&hashTable, words);
    }

    free(words);
    free(buffer);

    timespec endTime = {};
    clock_gettime(CLOCK_MONOTONIC, &endTime);

    printf("Time: %lg ms\n", 1e3 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_nsec - startTime.tv_nsec) / 1e6);

    return 0;
}

const char** readWords(char** buffer)
{
    FILE* file = fopen("res/02_optimize/words.txt", "r");
    assert(file);

    const char** words = (const char**) calloc(WORDS_COUNT, sizeof(const char*));
    assert(words);

    size_t bufferSize = readBufferFromFile(file, buffer);
    assert(*buffer);

    (*buffer)[bufferSize] = '\0';
    fclose(file);

    char* curPos = *buffer;
    for (uint32_t i = 0; i < WORDS_COUNT && *curPos != '\0'; i++)
    {
        words[i] = curPos;

        while (*curPos != '\n' && *curPos != '\0') { curPos++; }

        *curPos = '\0';
        curPos++;
    }

    return words;
}

void testInsertions(HashTable* hashTable, const char** words)
{
    assert(hashTable);
    assert(hashTable->buckets);
    assert(words);

    for (uint32_t i = 0; i < WORDS_COUNT; i++)
    {
        insert(hashTable, words[i], {words[i], 0, nullptr});
    }
}

void testSearches(HashTable* hashTable, const char** words)
{
    assert(hashTable);
    assert(hashTable->buckets);
    assert(words);

    for (uint32_t i = 0; i < WORDS_COUNT; i++)
    {
        find(hashTable, words[i]);
    }
}