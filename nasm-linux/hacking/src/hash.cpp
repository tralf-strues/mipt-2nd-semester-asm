#include "hash.h"

int writeHashToFile(FILE* file, uint32_t hash)
{
    if (file == nullptr)                               { return -1; }
    if (fwrite(&hash, sizeof(uint32_t), 1, file) == 0) { return -1; }

    return 0;
}

int readHashFromFile(FILE* file, uint32_t* hash)
{
    assert(hash);

    if (file == nullptr) { return -1; }

    uint32_t hashFromFile = 0;
    if (fread(&hashFromFile, 1, sizeof(uint32_t), file) == 0)
    {
        return -1;
    }

    *hash = hashFromFile;

    return 0;
}

uint32_t getHashCrc32(const unsigned char* buffer, size_t bytesCount)
{   
    assert(buffer);
    assert(bytesCount > 0);

    unsigned int hash = 0;    
    for (size_t i = 0; i < bytesCount; i++)
    {
        hash = (hash << 8) ^ CRC32_TABLE[((hash >> 24) ^ buffer[i]) & 255];
    }

    return hash;        
}