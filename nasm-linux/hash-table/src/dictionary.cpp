#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include "dictionary.h"
#include "hash_functions.h"

const size_t DEFAULT_DEFINITIONS_CAPACITY  = 3;
const float  DEFINITIONS_EXPAND_MULTIPLIER = 1.8;
const size_t MAX_WORD_LENGTH               = 128;

struct Parser
{
    char*  buffer;
    size_t bufferSize;
    char*  pos;
};

void       initDictionary      (Dictionary* dictionary, size_t hashTableSize, 
                                uint32_t (*getHash) (const char* value));
char*      parseWord           (Parser* parser);
Definition parseDefinition     (Parser* parser);
void       parsePartOfSpeech   (Parser* parser, Definition* definition);
void       parseDefinitionText (Parser* parser, Definition* definition);

void initDictionary(Dictionary* dictionary)
{
    assert(dictionary);
    initDictionary(dictionary, HASH_TABLE_SIZE, getMurmur3Hash);
}

void initDictionary(Dictionary* dictionary, size_t hashTableSize, 
                    uint32_t (*getHash) (const char* value))
{
    assert(dictionary);
    assert(hashTableSize > 0);
    assert(getHash);
    
    construct(&dictionary->hashTable, hashTableSize, strcmp, getHash); // FIXME: strcasecmp
    
    dictionary->wordsCount       = 0;
    dictionary->definitionsCount = 0;
    dictionary->definitions      = nullptr;
}

void destroyDictionary(Dictionary* dictionary)
{
    assert(dictionary);

    destroy(&(dictionary->hashTable));
    if (dictionary->definitions != nullptr) { free(dictionary->definitions); }

    dictionary->wordsCount       = 0;
    dictionary->definitionsCount = 0;
    dictionary->definitions      = nullptr;
}

void loadDictionary(Dictionary* dictionary, char* buffer, size_t bufferSize)
{
    assert(dictionary);
    assert(buffer);
    assert(bufferSize > 0);

    Parser    parser                 = {buffer, bufferSize, buffer};
    size_t    curDefinitionsCapacity = DEFAULT_DEFINITIONS_CAPACITY;
    DictEntry curEntry               = {}; 

    while (parser.pos - parser.buffer < bufferSize)
    {
        const char* word       = parseWord(&parser);
        Definition  definition = parseDefinition(&parser);

        if (curEntry.word != nullptr && strcasecmp(curEntry.word, word) == 0)
        {
            if (curEntry.definitionsCount + 1 > curDefinitionsCapacity)
            {
                curDefinitionsCapacity *= DEFINITIONS_EXPAND_MULTIPLIER;
                Definition* newDefinitions = (Definition*) realloc(curEntry.definitions, curDefinitionsCapacity * sizeof(Definition));
                assert(newDefinitions);
                curEntry.definitions = newDefinitions;
            }

            curEntry.definitions[curEntry.definitionsCount++] = definition;
        } 
        else
        {
            if (curEntry.word != nullptr)
            {
                insert(&(dictionary->hashTable), curEntry.word, curEntry);
                dictionary->wordsCount++;
            }

            curDefinitionsCapacity    = DEFAULT_DEFINITIONS_CAPACITY;
            curEntry.word             = word;
            curEntry.definitionsCount = 1;
            curEntry.definitions      = (Definition*) calloc(curDefinitionsCapacity, 
                                                             sizeof(Definition));
            assert(curEntry.definitions);
            curEntry.definitions[0] = definition;
        }
    }
}

const DictEntry* getDictionaryEntry(const Dictionary* dictionary, const char* word)
{
    assert(dictionary);
    assert(word);

    static char lowerWord[MAX_WORD_LENGTH + 1];

    size_t i = 0;
    for (; word[i] != '\0' && i < MAX_WORD_LENGTH; i++)
    {
        lowerWord[i] = tolower(word[i]);
    }

    lowerWord[i] = '\0';

    return find(&(dictionary->hashTable), lowerWord);
}

void printDictEntry(FILE* output, const DictEntry* dictEntry)
{
    assert(output);
    assert(dictEntry);
    assert(dictEntry->definitions);
    assert(dictEntry->definitionsCount > 0);

    for (uint32_t i = 0; i < dictEntry->definitionsCount; i++)
    {
        const Definition* curDefintion = dictEntry->definitions + i;
        assert(curDefintion);

        fprintf(output, "%u. [%s] %s\n", i + 1, POS_STRINGS[curDefintion->partOfSpeech], curDefintion->text);
    }
}

char* parseWord(Parser* parser)
{
    assert(parser);
    assert(parser->buffer);
    assert(parser->pos - parser->buffer < parser->bufferSize);

    char* word = parser->pos;
    word[0] = tolower(word[0]); // to make the first letter lower case
    
    char* wordEnd = strchr(word, ','); // FIXME: magic number
    assert(wordEnd);

    *wordEnd = '\0'; // put 0 due to null string termination
    parser->pos = wordEnd + 1;

    return word;
}

Definition parseDefinition(Parser* parser)
{
    assert(parser);
    assert(parser->buffer);
    assert(parser->pos - parser->buffer < parser->bufferSize);

    Definition definition = {};

    parsePartOfSpeech   (parser, &definition);
    parseDefinitionText (parser, &definition);

    return definition;
}

void parsePartOfSpeech(Parser* parser, Definition* definition) 
{
    assert(parser);
    assert(parser->buffer);
    assert(parser->pos - parser->buffer < parser->bufferSize);
    assert(definition);

    char* partOfSpeechStart = parser->pos;  
    char* partOfSpeechEnd   = strchr(partOfSpeechStart, ',');
    assert(partOfSpeechEnd);

    *partOfSpeechEnd = '\0'; // put 0 for strstr not to search after ','    
    definition->partOfSpeech = UNSPECIFIED;

    if (partOfSpeechStart != partOfSpeechEnd)
    {
        for (uint32_t i = 0; i < PARTS_OF_SPEECH_TOTAL - 1; i++)
        {
            if (strstr(partOfSpeechStart, POS_SPECIFIERS[i]) != nullptr)
            {
                // +1 is used due to the first PartOfSpeech being UNSPECIFIED
                definition->partOfSpeech = (PartOfSpeech) (i + 1);
                break; 
            }
        }
    }

    parser->pos = partOfSpeechEnd + 1;
}

void parseDefinitionText(Parser* parser, Definition* definition) 
{
    assert(parser);
    assert(parser->buffer);
    assert(parser->pos - parser->buffer < parser->bufferSize);
    assert(definition);

    definition->text = parser->pos;

    bool quoted = *(definition->text) == '"';
    if (quoted) { (definition->text)++; }  

    char* newLineSymb = strchr(definition->text, '\n');
    assert(newLineSymb);

    // if quoted then change '"' to 0, otherwise change '\n' to 0
    *(newLineSymb - (quoted ? 1 : 0)) = '\0'; 

    parser->pos = newLineSymb + 1;
}

