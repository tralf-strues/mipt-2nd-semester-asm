//------------------------------------------------------------------------------
//! Contains definitions of functions for file handling.
//! 
//! @file   file_loader.cpp
//! @author tralf-strues
//! @date   2021-04-03
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include <assert.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "file_loader.h"

size_t getFileSize(FILE* file)
{
    assert(file);

    struct stat fileInfo = {};
    fstat(fileno(file), &fileInfo);
    
    return fileInfo.st_size;
}

size_t readBufferFromFile(FILE* file, char** buffer)
{
    assert(file);
    assert(buffer);

    size_t bufferSize = getFileSize(file);
    *buffer = (char*) calloc(bufferSize, sizeof(char));
    assert(*buffer);

    fread(*buffer, sizeof(char), bufferSize, file);

    return bufferSize;
}