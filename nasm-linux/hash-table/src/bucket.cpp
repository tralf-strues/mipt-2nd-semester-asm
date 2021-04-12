#include "bucket.h"

Bucket* construct(Bucket* bucket, size_t capacity)
{
    assert(bucket);
    assert(capacity > 0);

#ifdef AVX_STRING_OPTIMIZATION
    bucket->data = (Pair*) aligned_alloc(32, capacity * sizeof(Pair));
#else 
    bucket->data = (Pair*) calloc(capacity, sizeof(Pair));
#endif

    if (bucket->data == nullptr) { return nullptr; }

    bucket->size     = 0;
    bucket->capacity = capacity;

    return bucket;
}

Bucket* construct(Bucket* bucket)
{
    assert(bucket);

    return construct(bucket, BUCKET_DEFAULT_CAPACITY);
}

void destroy(Bucket* bucket)
{
    assert(bucket);

    bucket->size     = 0;
    bucket->capacity = 0;

    free(bucket->data);
    bucket->data = nullptr;
}

Bucket* newBucket(size_t capacity)
{
    assert(capacity > 0);

    Bucket* bucket = (Bucket*) calloc(1, sizeof(Bucket));
    if (bucket->data == nullptr) { return nullptr; }

    return construct(bucket, capacity);
}

Bucket* newBucket()
{
    return newBucket(BUCKET_DEFAULT_CAPACITY);
}

void deleteBucket(Bucket* bucket)
{
    assert(bucket);

    destroy(bucket);
    free(bucket);
}

void resize(Bucket* bucket, size_t newCapacity)
{
    assert(bucket);
    assert(bucket->data);
    assert(bucket->size <= bucket->capacity);

#ifdef AVX_STRING_OPTIMIZATION
    Pair* newData = (Pair*) aligned_alloc(32, newCapacity * sizeof(Pair));
    assert(newData);

    memcpy(newData, bucket->data, bucket->size);
    free(bucket->data);
#else 
    Pair* newData = (Pair*) realloc(bucket->data, newCapacity * sizeof(Pair));
    assert(newData);
#endif

    bucket->data     = newData;
    bucket->capacity = newCapacity;
}

void shrinkToFit(Bucket* bucket)
{
    assert(bucket);
    assert(bucket->data);
    assert(bucket->size <= bucket->capacity);

    resize(bucket, bucket->size);
}

const Pair* get(Bucket* bucket, size_t i)
{
    assert(bucket);
    assert(bucket->data);
    assert(bucket->size <= bucket->capacity);
    assert(i < bucket->capacity);

    return &bucket->data[i];
}

void set(Bucket* bucket, size_t i, Pair pair)
{
    assert(bucket);
    assert(bucket->data);
    assert(bucket->size <= bucket->capacity);
    assert(i < bucket->capacity);

    bucket->data[i] = pair;
}

size_t spaceLeft(Bucket* bucket)
{
    assert(bucket);
    assert(bucket->data);
    assert(bucket->size <= bucket->capacity);

    return bucket->capacity - bucket->size;
}

void pushBack(Bucket* bucket, Pair pair)
{
    assert(bucket);
    assert(bucket->data);
    assert(bucket->size <= bucket->capacity);

    if (bucket->size + 1 > bucket->capacity)
    {
        resize(bucket, bucket->capacity * BUCKET_EXPAND_MULTIPLIER);
    }

    set(bucket, bucket->size++, pair);
}