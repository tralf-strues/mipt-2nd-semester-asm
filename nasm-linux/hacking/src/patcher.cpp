#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "patcher.h"
#include "hash.h"
#include "file_loader.h"

int init(Patcher* patcher, const char* filename)
{
    assert(patcher);
    assert(filename);

    FILE* file = fopen(filename, "r");
    if (file != nullptr)
    {
        patcher->file     = file;
        patcher->filename = filename;
    }

    return file == nullptr ? -1 : 0;
}

void destroy(Patcher* patcher)
{
    assert(patcher);

    patcher->file     = nullptr;
    patcher->filename = nullptr;

    fclose(patcher->file);
}

IntegrityCheckStatus checkExecIntegrity(Patcher* patcher)
{
    assert(patcher);

    FILE* refHashFile = fopen(REF_HASH_FILENAME, "r");

    uint32_t correctHash = 0;
    if (refHashFile == nullptr || readHashFromFile(refHashFile, &correctHash) == -1)
    {
        return INTEGRITY_CHECK_REF_HASH_FILE_ERROR;
    }     

    size_t bufferSize = 0;
    unsigned char* buffer = readBufferFromFile(patcher->file, &bufferSize);

    if (buffer == nullptr)
    {
        return INTEGRITY_CHECK_FILE_ERROR;
    }

    patcher->buffer     = buffer;
    patcher->bufferSize = bufferSize;

    fclose(patcher->file);
    patcher->file = fopen(PATCHED_EXEC_FILENAME, "w");

    if (patcher->file == nullptr)
    {
        return INTEGRITY_CHECK_FILE_ERROR;
    }

    return getHashCrc32(buffer, bufferSize) == correctHash ? INTEGRITY_CHECK_HASH_CORRECT : 
                                                             INTEGRITY_CHECK_HASH_INCORRECT;
}

void patch(Patcher* patcher)
{
    assert(patcher);
    assert(patcher->buffer);

    patcher->buffer[BYTE_TO_CHANGE_POS] = BYTE_TO_CHANGE_RES;
}