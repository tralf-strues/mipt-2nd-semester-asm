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

typedef const char* ht_key_t;
typedef DictEntry   ht_value_t; 

struct Pair
{
    ht_key_t   key;
    ht_value_t value;
};

const size_t BUCKET_DEFAULT_CAPACITY  = 2;
const double BUCKET_EXPAND_MULTIPLIER = 1.7;

struct Bucket
{
    Pair*  data;
    size_t size;
    size_t capacity;
};

//------------------------------------------------------------------------------
//! Constructs bucket with specified capacity.
//! 
//! @param bucket
//! @param capacity
//! 
//! @note Memory allocation of capacity * sizeof(Pair) bytes is called. 
//!
//! @warning Capacity must be > 0! 
//! 
//! @return Constructed bucket or nullptr if an error occurred.
//------------------------------------------------------------------------------
Bucket* construct(Bucket* bucket, size_t capacity);

//------------------------------------------------------------------------------
//! Constructs bucket with capacity BUCKET_DEFAULT_CAPACITY.
//! 
//! @param bucket
//! 
//! @note Memory allocation of BUCKET_DEFAULT_CAPACITY * sizeof(Pair) bytes is 
//!       called.
//! 
//! @return Constructed bucket or nullptr if an error occurred.
//------------------------------------------------------------------------------
Bucket* construct(Bucket* bucket);

//------------------------------------------------------------------------------
//! Frees used memory and resets bucket's values.
//! 
//! @param bucket
//------------------------------------------------------------------------------
void destroy(Bucket* bucket);

//------------------------------------------------------------------------------
//! Allocate memory for a bucket of specified capacity and construct it. 
//! 
//! @param capacity
//! 
//! @return New bucket of nullptr if an error occurred.
//------------------------------------------------------------------------------
Bucket* newBucket(size_t capacity);

//------------------------------------------------------------------------------
//! Allocate memory for a bucket of BUCKET_DEFAULT_CAPACITY capacity and 
//! construct it. 
//! 
//! @return New bucket of nullptr if an error occurred.
//------------------------------------------------------------------------------
Bucket* newBucket();

//------------------------------------------------------------------------------
//! Deletes a bucket creates using newBucket().
//! 
//! @param bucket
//! 
//! @warning Bucket has to be created using newBucket(). Otherwise it's 
//!          undefined behavior.
//------------------------------------------------------------------------------
void deleteBucket(Bucket* bucket);

//------------------------------------------------------------------------------
//! Resizes bucket with new capacity.
//! 
//! @param bucket
//! @param newCapacity
//! 
//! @warning If newCapacity is less than the old capacity then there will be 
//!          data loss!
//------------------------------------------------------------------------------
void resize(Bucket* bucket, size_t newCapacity);

//------------------------------------------------------------------------------
//! Resizes bucket so that its new capacity is equal to its size.  
//! 
//! @param bucket
//------------------------------------------------------------------------------
void shrinkToFit(Bucket* bucket);

//------------------------------------------------------------------------------
//! Get ith Pair in bucket.
//! 
//! @param bucket
//! @param i
//! 
//! @return Constant pointer to ith Pair in bucket.
//------------------------------------------------------------------------------
const Pair* get(const Bucket* bucket, size_t i);

//------------------------------------------------------------------------------
//! Sets ith Pair in bucket to pair.
//! 
//! @param bucket
//! @param i
//! @param pair
//------------------------------------------------------------------------------
void set(Bucket* bucket, size_t i, Pair pair);

//------------------------------------------------------------------------------
//! @param bucket
//! 
//! @return How much unused space is left in bucket. Equal to bucket's capacity
//!         minus its size.
//------------------------------------------------------------------------------
size_t spaceLeft(const Bucket* bucket);

//------------------------------------------------------------------------------
//! Adds pair to the end of bucket and if necessary reallocates memory in case
//! bucket's size reaches its capacity.
//! 
//! @param bucket
//! @param pair
//------------------------------------------------------------------------------
void pushBack(Bucket* bucket, Pair pair);

//------------------------------------------------------------------------------
//! Returns the last pair in bucket
//! 
//! @param bucket
//! 
//! @return Constant pointer to the last pair in bucket, or nullptr if an
//!         error occurres or bucket is empty.
//------------------------------------------------------------------------------
const Pair* popBack (Bucket* bucket);