#pragma once

static const char*  REF_HASH_FILENAME     = "correct_hash.hash";
static const char*  PATCHED_EXEC_FILENAME = "PATCHED.com";
static const size_t BYTE_TO_CHANGE_POS    = 0x25; /* jnz=0x75 --> jmp=0xEB */
static const size_t BYTE_TO_CHANGE_VAL    = 0x75;
static const size_t BYTE_TO_CHANGE_RES    = 0xEB;
 
struct Patcher
{
    const char*    filename;
    unsigned char* buffer;
    size_t         bufferSize;
    FILE*          file;
};

enum IntegrityCheckStatus
{
    INTEGRITY_CHECK_HASH_CORRECT,
    INTEGRITY_CHECK_HASH_INCORRECT,
    INTEGRITY_CHECK_REF_HASH_FILE_ERROR,
    INTEGRITY_CHECK_FILE_ERROR
};

int                  init               (Patcher* patcher, const char* filename);
void                 destroy            (Patcher* patcher);
IntegrityCheckStatus checkExecIntegrity (Patcher* patcher);
void                 patch              (Patcher* patcher);