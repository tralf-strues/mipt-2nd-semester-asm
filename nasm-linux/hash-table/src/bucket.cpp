#include "bucket.h"

Bucket* construct(Bucket* bucket, size_t capacity)
{
    assert(bucket);
    assert(capacity > 0);

    bucket->data = (value_t*) calloc(capacity, sizeof(value_t));
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

    value_t* newData = (value_t*) realloc(bucket->data, newCapacity * sizeof(value_t));
    assert(newData);

    bucket->data = newData;
    bucket->size = newCapacity;
}

void shrinkToFit(Bucket* bucket)
{
    assert(bucket);

    resize(bucket, bucket->size);
}

value_t* get(Bucket* bucket, size_t i)
{
    assert(bucket);
    assert(i < bucket->capacity);

    return &bucket->data[i];
}

void set(Bucket* bucket, size_t i, value_t value)
{
    assert(bucket);
    assert(i < bucket->capacity);

    bucket->data[i] = value;
}

size_t spaceLeft(Bucket* bucket)
{
    assert(bucket);

    return bucket->capacity - bucket->size;
}

void pushBack(Bucket* bucket, value_t value)
{
    assert(bucket);

    if (bucket->size + 1 > bucket->capacity)
    {
        resize(bucket, bucket->capacity * BUCKET_EXPAND_MULTIPLIER);
    }

    set(bucket, bucket->size++, value);
}

value_t* popBack(Bucket* bucket)
{
    assert(bucket);

    return get(bucket, bucket->size--);
}