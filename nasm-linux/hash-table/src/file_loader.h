//------------------------------------------------------------------------------
//! Contains declarations of functions for file handling.
//! 
//! @file   file_loader.h
//! @author tralf-strues
//! @date   2021-04-03
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#pragma once
#include <stdio.h>

//------------------------------------------------------------------------------
//! @param file
//! 
//! @warning File has to be already open with fopen().
//!
//! @return File's size in bytes.
//------------------------------------------------------------------------------
size_t getFileSize(FILE* file);

//------------------------------------------------------------------------------
//! Reads the entire content of file into the buffer.
//! 
//! @param file
//! @param buffer
//! 
//! @warning Buffer will be allocated based on the file's size, so it should
//!          not have been allocated prior to this function.
//! @warning File has to be already open with fopen().
//! 
//! @return Size in bytes of the buffer.
//------------------------------------------------------------------------------
size_t readBufferFromFile(FILE* file, char** buffer);