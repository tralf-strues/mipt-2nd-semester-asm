#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "file_loader.h"

unsigned char* readBufferFromFile(FILE* file, size_t* bufferSize)
{
    assert(file);
    assert(bufferSize);

    *bufferSize = getFileSize(file);
    unsigned char* buffer = (unsigned char*) calloc(*bufferSize, sizeof(unsigned char));
    assert(buffer);

    fread(buffer, sizeof(unsigned char),  *bufferSize, file);

    return buffer;
}

size_t getFileSize(FILE* file)
{
    assert(file);

    struct stat fileInfo = {};
    fstat(fileno(file), &fileInfo);
    
    return fileInfo.st_size;
}