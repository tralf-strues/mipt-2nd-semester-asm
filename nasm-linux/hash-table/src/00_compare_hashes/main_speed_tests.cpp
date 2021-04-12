#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "../hash_functions.h"

struct Sample
{
    const char* testString;
    const char* testName;
    size_t testStringLength; 
};

const char* STRING_SENTENCE = "Evil is evil. Lesser, greater, middling, it's all the same. "
                              "Proportions are negotiated, boundaries blurred. I'm not a "
                              "pious hermit. I haven't done only good in my life. But if "
                              "I'm to choose between one evil and another, then I prefer "
                              "not to choose at all.";
const size_t SAMPLES_COUNT = 5;

const Sample SAMPLES[SAMPLES_COUNT] = {
        { "The",             "Half average-length word",   3   },
        { "Meow!",           "Average-length word",        5   },
        { "Witchcraft",      "Double average-length word", 10  },
        { "Conceptualizing", "Triple average-length word", 15  },
        { STRING_SENTENCE,   "Sentence",                   255 },
    };

const size_t TESTS_COUNT = 1e7;

void testHashFunction(uint32_t (*getHash)(const char* string), const char* hashName);

int main()
{
    testHashFunction(getConstantHash,       "constant hash");
    testHashFunction(getStrLenHash,         "strlen hash");
    testHashFunction(getFirstCharHash,      "first char hash");
    testHashFunction(getSumCharsHash,       "sum chars hash");
    testHashFunction(getXorRightRotateHash, "xor right rotate hash");
    testHashFunction(getXorLeftRotateHash,  "xor left rotate hash");
    testHashFunction(getMurmur3Hash,        "murmur3 hash");
    testHashFunction(getCrc32Hash,          "crc32 hash");
    testHashFunction(getOPCrc32Hash,        "optimized crc32 hash");

    return 0;
}

void testHashFunction(uint32_t (*getHash)(const char* string), const char* hashName)
{
    assert(getHash);
    assert(hashName);

    timespec startTime = {};
    timespec endTime   = {};

    printf("\nTesting %s\n", hashName);

    for (uint32_t i = 0; i < SAMPLES_COUNT; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &startTime);

        uint32_t hashValue = 0;
        for (uint32_t j = 0; j < TESTS_COUNT; j++)
        {
            hashValue = getHash(SAMPLES[i].testString);
        }

        clock_gettime(CLOCK_MONOTONIC, &endTime);

        double deltaTime = 1e9 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_nsec - startTime.tv_nsec);

        printf("Sample %u [%zu]:\t%lg ns\t(%s)\n", 
               i, 
               SAMPLES[i].testStringLength,
               deltaTime / TESTS_COUNT,
               SAMPLES[i].testName);
    }
}