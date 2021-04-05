#include <assert.h>
#include <math.h>
#include "../file_loader.h"
#include "../dictionary.h"
#include "../hash_functions.h"

const size_t HASH_TABLE_SIZES_COUNT = 5;
const size_t SAMPLES_COUNT          = 7;
const double MAX_BAR_HEIGHT_MULT    = 1.5;
const size_t BARS_COUNT             = 256;
const size_t MAX_FILENAME_LENGTH    = 256;

size_t HASH_TABLE_SIZES[HASH_TABLE_SIZES_COUNT] = {
            521,
            1031,
            2053,
            4099,
            8209
        }; 

uint32_t (*const HASH_FUNCTIONS[SAMPLES_COUNT]) (const char* string) = {
            getConstantHash,
            getStrLenHash,
            getFirstCharHash,
            getSumCharsHash,
            getXorRightRotateHash,
            getXorLeftRotateHash,
            getMurmur3Hash
        };

const char* SAMPLES_NAMES[SAMPLES_COUNT] = {
            "Constant hash function",
            "String length hash function",
            "First character hash function",
            "Characters sum hash function",
            "Xor and right rotate hash function",
            "Xor and left rotate hash function",
            "Murmur3 hash function"
        };

struct TestSample
{
    Dictionary dictionary;
    char*      buffer;
    double     standardDeviation;
};

void runTests            (size_t hashTableSize);
void loadDictionaries    (TestSample samples[], size_t hashTableSize);
void destroyDictionaries (TestSample samples[]);

void writePlotData       (TestSample samples[]);
void writeGnuplotScript  (TestSample samples[]);
void runGnuplotScript    (size_t hashTableSize);

int main()
{
    for (uint32_t i = 0; i < HASH_TABLE_SIZES_COUNT; i++)
    {
        runTests(HASH_TABLE_SIZES[i]);
    }

    return 0;
}

void runTests(size_t hashTableSize)
{
    TestSample samples[SAMPLES_COUNT] = {};    
    loadDictionaries(samples, hashTableSize);

    printf("Dictionaries loaded, wordsCount = %zu.\n", samples[0].dictionary.wordsCount);

    writePlotData(samples);
    writeGnuplotScript(samples);
    runGnuplotScript(hashTableSize);

    destroyDictionaries(samples);
}

void loadDictionaries(TestSample samples[], size_t hashTableSize)
{
    assert(samples);
    assert(hashTableSize > 0);

    FILE* file = fopen("res/00_compare_hashes/english_dict_short.csv", "r");
    assert(file);

    size_t bufferSize = getFileSize(file);
    readBufferFromFile(file, &(samples[0].buffer));

    for (uint32_t i = 1; i < SAMPLES_COUNT; i++)
    {
        samples[i].buffer = (char*) calloc(bufferSize, sizeof(char));
        assert(samples[i].buffer);
    }

    for (uint32_t i = 0; i < SAMPLES_COUNT; i++)
    {
        if (i + 1 < SAMPLES_COUNT)
        {
            memcpy(samples[i + 1].buffer, samples[i].buffer, bufferSize);
        }

        initDictionary(&(samples[i].dictionary), hashTableSize, HASH_FUNCTIONS[i]);
        loadDictionary(&(samples[i].dictionary), samples[i].buffer, bufferSize);
    }

    fclose(file);
}

void destroyDictionaries(TestSample samples[])
{
    assert(samples);

    for (uint32_t i = 0; i < SAMPLES_COUNT; i++)
    {
        destroyDictionary(&(samples[i].dictionary));
        free(samples[i].buffer);

        samples[i].buffer  = nullptr;
    }
}

void writePlotData(TestSample samples[])
{
    assert(samples);

    char filename[MAX_FILENAME_LENGTH] = {};
    snprintf(filename, 
             MAX_FILENAME_LENGTH, 
             "res/00_compare_hashes/size_%zu/plot_data.dat",
             samples[0].dictionary.hashTable.size);

    FILE* file = fopen(filename, "w");
    assert(file);

    size_t maxBucketSize  = 0;
    double meanBucketSize = (double) samples[0].dictionary.wordsCount / 
                            (double) samples[0].dictionary.hashTable.size; 

    for (uint32_t i = 0; i < SAMPLES_COUNT; i++)
    {
        const Bucket* buckets      = samples[i].dictionary.hashTable.buckets;
        const size_t  bucketsCount = samples[i].dictionary.hashTable.size;

        size_t bucketsPerBar = bucketsCount / BARS_COUNT;

        double standardDeviation = 0;

        uint32_t bucketIdx = 0;
        for (uint32_t bar = 0; bar < BARS_COUNT; bar++)
        {
            size_t sum = 0;
            for (uint32_t j = 0; j < bucketsPerBar; j++)
            {
                size_t curBucketSize = buckets[bar * bucketsPerBar + j].size;
                sum += curBucketSize;
                standardDeviation += (meanBucketSize - curBucketSize) *
                                     (meanBucketSize - curBucketSize); 
            }

            fprintf(file, "%lu %lg\n", bar * bucketsPerBar, (double) sum / (double) bucketsPerBar); 
        }

        size_t sum = 0;
        for (uint32_t j = 0; j < bucketsCount % BARS_COUNT; j++)
        {
            size_t curBucketSize = buckets[(BARS_COUNT - 1) * bucketsPerBar + j].size;
            sum += curBucketSize;
            standardDeviation += (meanBucketSize - curBucketSize) *
                                 (meanBucketSize - curBucketSize); 
        }

        standardDeviation /= bucketsCount;
        standardDeviation = sqrt(standardDeviation);
        samples[i].standardDeviation = standardDeviation;

        fprintf(file, "%lu %lg\n\n\n", (BARS_COUNT - 1) * bucketsPerBar, (double) sum / 
                                                                         (double) bucketsPerBar); 
    }

    fclose(file);
}

void writeGnuplotScript(TestSample samples[])
{
    assert(samples);

    size_t hashTableSize = samples[0].dictionary.hashTable.size;

    char filename[MAX_FILENAME_LENGTH] = {};
    snprintf(filename, 
             MAX_FILENAME_LENGTH, 
             "res/00_compare_hashes/size_%zu/gnu_plot_script.gp",
             hashTableSize);

    FILE* file = fopen(filename, "w");
    assert(file);

    fprintf(file, "set key noautotitle\n"
                  "set xlabel \"Bucket\" font \",38\" offset 0, -1\n"
                  "set ylabel \"Bucket size\" font \",38\" offset -5, 0\n"
                  "set size ratio 0.4\n"
                  "set datafile separator whitespace\n"
                  "set title font \",48\"\n"
                  "set xtics font \",36\"\n"
                  "set ytics font \",36\"\n"
                  "set style fill solid border lt -1\n"
                  "set style line 1 lc rgb 0x6495ED\n"
                  "set xrange[0:%zu]\n\n",
                  hashTableSize);

    for (uint32_t i = 0; i < SAMPLES_COUNT; i++)
    {
        fprintf(file, "set term svg dynamic size 5000 2000 background rgb \"white\"\n"
                      "set key title \"Standard deviation = %lg\" font \",40\" offset 0, -3\n"
                      "set output \"res/00_compare_hashes/size_%zu/gnuplot_test_output%u.svg\"\n"
                      "set title \"%s\"\n"
                      "plot \"res/00_compare_hashes/size_%zu/plot_data.dat\" using 1:2 index %u with boxes ls 1\n\n",
                      samples[i].standardDeviation,
                      hashTableSize,
                      i + 1,
                      SAMPLES_NAMES[i],
                      hashTableSize,
                      i);
    }

    fclose(file);
}

void runGnuplotScript(size_t hashTableSize)
{
    assert(hashTableSize > 0);

    char filename[MAX_FILENAME_LENGTH] = {};
    snprintf(filename, 
             MAX_FILENAME_LENGTH, 
             "gnuplot -c res/00_compare_hashes/size_%zu/gnu_plot_script.gp",
             hashTableSize);

    system(filename);
}