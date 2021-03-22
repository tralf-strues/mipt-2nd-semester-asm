#pragma once

#include <stdio.h>

unsigned char* readBufferFromFile (FILE* file, size_t* bufferSize);
size_t         getFileSize        (FILE* file); 