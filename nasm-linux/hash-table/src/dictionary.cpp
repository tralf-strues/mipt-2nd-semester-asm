#include <string.h>
#include <stdint.h>
#include "dictionary.h"

const size_t DEFAULT_DEFINITIONS_CAPACITY  = 3;
const float  DEFINITIONS_EXPAND_MULTIPLIER = 1.8;

struct Parser
{
    char*  buffer;
    size_t bufferSize;
    char*  pos;
};

// TODO: add functions' declarations

int  cmpDictEntries (const DictEntry* first, const DictEntry* second);
void initDictionary (Dictionary* dictionary, uint32_t (*getHash)(key_t key));

int cmpDictEntries(const DictEntry* first, const DictEntry* second)
{
    assert(first);
    assert(second);

    return strcasecmp(first->word, second->word);
}

void initDictionary(Dictionary* dictionary)
{
    assert(dictionary);
    // FIXME: add default hash-function
    initDictionary(dictionary, nullptr);
}

void initDictionary(Dictionary* dictionary, uint32_t (*getHash)(key_t key))
{
    assert(dictionary);
    assert(getHash);
    
    construct(&dictionary->hashTable, HASH_TABLE_SIZE, cmpDictEntries, getHash); 
    
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

char* parseWord(Parser* parser)
{
    assert(parser);
    assert(parser->buffer);
    assert(parser->pos - parser->buffer < parser->bufferSize);

    char* word    = parser->pos;
    char* wordEnd = strchr(word, ','); // FIXME: magic number
    *wordEnd = 0; // put 0 due to null string termination

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
    *partOfSpeechEnd = 0; // put 0 for strstr not to search after ','
    
    definition->partOfSpeech = UNSPECIFIED;

    if (partOfSpeechStart != partOfSpeechEnd)
    {
        for (uint32_t i = 0; i < PARTS_OF_SPEECH_TOTAL - 1; i++)
        {
            if (strstr(partOfSpeechStart, POS_SPECIFIERS[i]) != nullptr)
            {
                definition->partOfSpeech = (PartOfSpeech) (UNSPECIFIED + i);
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

    // if quoted then change '"' to 0, otherwise change '\n' to 0
    *(newLineSymb - (quoted ? 1 : 0)) = 0; 

    parser->pos = newLineSymb + 1;
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
                realloc(curEntry.definitions, curDefinitionsCapacity * sizeof(Definition));
            }

            curEntry.definitions[curEntry.definitionsCount++] = definition;
        } 
        else
        {
            if (curEntry.word != nullptr)
            {
                insert(&(dictionary->hashTable), curEntry.word, curEntry);
            }

            curDefinitionsCapacity    = DEFAULT_DEFINITIONS_CAPACITY;
            curEntry.word             = word;
            curEntry.definitionsCount = 1;
            curEntry.definitions      = (Definition*) calloc(curDefinitionsCapacity, 
                                                             sizeof(Definition));
        }
    }
}

const DictEntry* getDictionaryEntry(const Dictionary* dictionary, const char* word)
{
    assert(dictionary);
    assert(word);

    return get(&(dictionary->hashTable), word);
}

