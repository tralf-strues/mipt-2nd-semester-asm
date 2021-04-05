#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../dictionary.h"
#include "../file_loader.h"
#include "html_writer.h"

// TODO:
// 1) keep in mind apostrophe "<Librarian>'s book"
// 2) in loading dictionary add ASSERT_PARSER
// 3) add ASSERT_BUCKET
// 4) Add links to paragraphs in README
// 5) Make a universal MAX_WORD_LENGTH constant

enum FlagId
{
    FLAG_HELP,
    FLAG_WORD,
    FLAG_DOC,
    FLAG_OUTPUT,

    FLAGS_TOTAL
};

struct InputHandler
{
    int    argc;
    char** argv;
    int    curArg;
    bool   stopped;

    const char* word;
    const char* input;
    const char* output;
};

struct Flag
{
    const char  shortVersion;
    const char* longVersion;
    void (*processFlag) (InputHandler* handler);
};

void printDictEntry    (const DictEntry* dictEntry);
int  processFlags      (InputHandler* handler);
void processFlagHelp   (InputHandler* handler);
void processFlagWord   (InputHandler* handler);
void processFlagDoc    (InputHandler* handler);
void processFlagOutput (InputHandler* handler);

const Flag FLAGS[FLAGS_TOTAL] = {
        { 'h', "help",   processFlagHelp   },
        { 'w', "word",   processFlagWord   },
        { 'd', "doc",    processFlagDoc    },
        { 'o', "output", processFlagOutput }
    };

const char* HELP_MESSAGE = "These are commands for define:\n"
                           "  -h, --help   [show this message]\n"
                           "  -w, --word   [define just one word]\n"
                           "  -d, --doc    [specify input document]\n"
                           "  -o, --output [specify output file]\n";

const char* DEFAULT_OUTPUT_FILENAME = "defined.html";

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        printf("Define: no command specified. See help using -h or --help.\n");
    }

    InputHandler handler = {argc, argv, 1, false, nullptr, nullptr};
    if (processFlags(&handler) == -1) { return -1; }

    FILE* databaseFile = fopen("res/english_dict.csv", "r");
    assert(databaseFile);

    char* buffer = nullptr;

    size_t bufferSize = readBufferFromFile(databaseFile, &buffer);
    buffer[bufferSize] = '\0';
    fclose(databaseFile);

    Dictionary dictionary = {};
    initDictionary(&dictionary);
    loadDictionary(&dictionary, buffer, bufferSize);

    if (handler.word != nullptr)
    {
        const DictEntry* dictEntry = getDictionaryEntry(&dictionary, handler.word);

        if (dictEntry == nullptr)
        {
            printf("There is no defintion for '%s'.\n", handler.word);
        }
        else 
        {
            printf("Definitions for '%s'\n", dictEntry->word);
            printDictEntry(stdout, dictEntry);
        }
    }

    if (handler.input != nullptr)
    {
        FILE* inputFile = fopen(handler.input, "r");
        if (inputFile == nullptr)
        {
            printf("File '%s' not found.\n", handler.input);
            
            destroyDictionary(&dictionary);
            free(buffer);
            return -1;
        }

        char* docBuffer = nullptr;
        readBufferFromFile(inputFile, &docBuffer);
        fclose(inputFile);

        FILE* outputFile = fopen(handler.output == nullptr ? DEFAULT_OUTPUT_FILENAME : 
                                                             handler.output, "w");
        if (outputFile == nullptr)
        {
            printf("Failed to open file '%s'.\n");

            destroyDictionary(&dictionary);
            free(buffer);
            free(docBuffer);
            return -1;
        }

        defineDoc(&dictionary, docBuffer, outputFile);
        
        free(docBuffer);
        fclose(outputFile);
    }

    destroyDictionary(&dictionary);
    free(buffer);

    return 0;
}

int processFlags(InputHandler* handler)
{
    assert(handler);

    while (handler->curArg < handler->argc)
    {
        const char* argument = handler->argv[handler->curArg]; 
        bool processed = false;

        if (argument[0] == '-' && argument[1] != '-')
        {
            for (uint32_t i = 0; i < FLAGS_TOTAL; i++)
            {
                if (argument[1] == FLAGS[i].shortVersion)
                {
                    FLAGS[i].processFlag(handler);
                    processed = true;
                }
            }
        }
        else if (argument[0] == '-')
        {
            for (uint32_t i = 0; i < FLAGS_TOTAL; i++)
            {
                if (strcmp(argument + 2, FLAGS[i].longVersion) == 0)
                {
                    FLAGS[i].processFlag(handler);
                    processed = true;
                }
            }
        }

        if (!processed)       { printf("Unknown flag.\n"); return -1; }
        if (handler->stopped) { break; }
    }

    return 0;
}

void processFlagHelp(InputHandler* handler)
{
    assert(handler);

    printf(HELP_MESSAGE);

    handler->stopped = true;
}

void processFlagWord(InputHandler* handler)
{  
    assert(handler);

    if (handler->curArg + 1 >= handler->argc)
    {
        printf("No word specified after flag -w. See help.\n");
        handler->stopped = true;

        return;
    }

    handler->word = handler->argv[handler->curArg + 1];
    handler->curArg += 2;
}

void processFlagDoc(InputHandler* handler)
{
    assert(handler);

    if (handler->curArg + 1 >= handler->argc)
    {
        printf("No input document specified. See help.\n");
        handler->stopped = true;
        
        return;
    }

    handler->input = handler->argv[handler->curArg + 1];
    handler->curArg += 2;
}

void processFlagOutput(InputHandler* handler)
{
    assert(handler);

    if (handler->curArg + 1 >= handler->argc)
    {
        printf("No output file specified. See help.\n");
        handler->stopped = true;
        
        return;
    }

    handler->output = handler->argv[handler->curArg + 1];
    handler->curArg += 2;
}