//------------------------------------------------------------------------------
//! Declares Bucket struct, which is a bucket used in the HashTable, and 
//! functions related to it. Basically, Bucket is just a simple dynamic array 
//! (also known as vector in C++ stl).
//! 
//! @file   bucket.h
//! @author tralf-strues
//! @date   2021-04-01
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "language.h"

typedef DictEntry value_t; 

const size_t BUCKET_DEFAULT_CAPACITY  = 2;
const double BUCKET_EXPAND_MULTIPLIER = 1.7;

struct Bucket
{
    value_t* data;
    size_t   size;
    size_t   capacity;
};

Bucket*  construct    (Bucket* bucket, size_t capacity);
Bucket*  construct    (Bucket* bucket);
void     destroy      (Bucket* bucket);

Bucket*  newBucket    (size_t capacity);
Bucket*  newBucket    ();
void     deleteBucket (Bucket* bucket);

void     resize       (Bucket*       bucket, size_t newCapacity);
void     shrinkToFit  (Bucket*       bucket);
value_t* get          (const Bucket* bucket, size_t i);
void     set          (Bucket*       bucket, size_t i, value_t value);

size_t   spaceLeft    (const Bucket* bucket);
void     pushBack     (Bucket*       bucket, value_t value);
value_t* popBack      (Bucket*       bucket);