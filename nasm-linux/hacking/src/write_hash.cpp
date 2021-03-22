#include <assert.h>
#include <stdio.h>

#include "file_loader.h"
#include "hash.h"

const char* DEFAULT_OUTPUT_FILENAME = "a.hash";

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        printf("No file to compute the hash of!\n");
        return -1;
    }

    FILE* inputFile = fopen(argv[1], "r");
    assert(inputFile);

    FILE* outputFile = fopen(argc > 2 ? argv[2] : DEFAULT_OUTPUT_FILENAME, "w");
    assert(inputFile);

    size_t         bufferSize = 0;
    unsigned char* buffer     = readBufferFromFile(inputFile, &bufferSize);
    assert(buffer != nullptr);

    uint32_t hashValue = getHashCrc32(buffer, bufferSize);
    writeHashToFile(outputFile, hashValue);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
