#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

// TODO:
// 1) keep in mind apostrophe "<Librarian>'s book"
// 2) in loading dictionary add ASSERT_PARSER

// FIXME: put it in a separate file
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

int main()
{
    FILE* file = fopen("res/english_dict.csv", "r");
    assert(file);

    printf("File opened\n");

    char*  buffer = nullptr;
    size_t bufferSize = readBufferFromFile(file, &buffer);

    printf("Loaded, bufferSize = %lu\n", bufferSize);
    printf("Buffer:\n%.*s", 512, buffer);

    fclose(file);
}