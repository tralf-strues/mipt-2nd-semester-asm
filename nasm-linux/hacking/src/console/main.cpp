#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "colored_output.h"
#include "../patcher.h"

const size_t MAX_FILENAME_LENGTH = 64;
const size_t PROGRESS_BAR_LENGTH = 25;
const size_t SLEEP_SHORT         = 25000;
const size_t SLEEP_MEDIUM        = 75000;
const size_t SLEEP_LONG          = 100000;

const char* MSG_HEADER = "===========================================================\n"
                         "!3! !8! !0! !2!            PATCH        (tralf-strues 2021)\n"
                         "===========================================================\n";

const char* ASCII_IMG_PIRATE = "\t\t     ..............   \n"         
                               "\t\t   :::::::::::::::::: \n"       
                               "\t\t  :::::::::::::::     \n"           
                               "\t\t :::`::::::: :::     :\n"        
                               "\t\t :::: ::::: :::::    :\n"      
                               "\t\t :`   :::::;     :..~~\n"         
                               "\t\t :   ::  :::.     :::.\n"            
                               "\t\t :...`:, :::::...:::  \n"         
                               "\t\t::::::.  :::::::::'   \n"           
                               "\t\t :::::::::::::::::  ! \n"       
                               "\t\t :;;;;;;;;;;;;;;;;']} \n"      
                               "\t\t ;--.--.--.--.--.-    \n"     
                               "\t\t  \\/ \\/ \\/ \\/ \\/ \\/   \n"      
                               "\t\t     :::       ::::   \n"    
                               "\t\t      :::      ::     \n"         
                               "\t\t     :\\:      ::      \n"      
                               "\t\t   /\\::    /\\:::      \n"       
                               "\t\t ^.:^:.^^^::`::       \n"       
                               "\t\t ::::::::.::::        \n"        
                               "\t\t  .::::::::::         \n\n";        
                         
void printHeader      ();
void askFilename      (char* filename);

int  initPatcher      (Patcher* patcher, const char* filename);
int  checkIntegrity   (Patcher* patcher);
void patchExecutable  (Patcher* patcher);
void saveExecutable   (Patcher* patcher);

void printExit        (bool successfull);
void printProgressBar (const char* message, useconds_t sleepMilliseconds);
void printStatusBox   (const char* text, size_t foregroundColor);
void printOk          ();
void printFailed      ();

int main()
{
    FILE* file = popen("aplay patcher_audio.wav &>/dev/null", "r");

    printHeader();

    char filename[MAX_FILENAME_LENGTH];
    askFilename(filename);

    Patcher patcher = {};
    if (initPatcher(&patcher, filename) == -1)
    {
        printExit(false);
        pclose(file);
        return -1;
    }

    if (checkIntegrity(&patcher) == -1)
    {
        printExit(false);
        pclose(file);
        fclose(patcher.file);
        return -1;
    }

    patchExecutable(&patcher);
    saveExecutable(&patcher);

    printExit(true);
    pclose(file);
    fclose(patcher.file);
    return 0;
}

void printHeader()
{
    setFgrColor(FGR_CYAN);
    printf(MSG_HEADER);
    resetColor();

    setFgrColor(FGR_YELLOW);
    setModColor(MOD_FLASHING);
    printf(ASCII_IMG_PIRATE);
    resetColor();
}

void askFilename(char* filename)
{
    assert(filename);

    printf("Enter the filename (in current directory): ");
    scanf("%s", filename);

    putchar('\n');
}

int initPatcher(Patcher* patcher, const char* filename)
{
    assert(patcher);
    assert(filename);

    printProgressBar("Opening the file...", SLEEP_SHORT);

    if (init(patcher, filename) == -1)
    {
        printFailed();
        printf("Couldn't open the file \"%s\".\n\n", filename);
        
        return -1;
    }

    printOk();
    printf("File \"%s\" successfully opened.\n\n", filename);

    return 0;
}

int checkIntegrity(Patcher* patcher)
{
    assert(patcher);

    printProgressBar("Checking file integrity...", SLEEP_MEDIUM);

    IntegrityCheckStatus checkResult = checkExecIntegrity(patcher);
    switch (checkResult)
    {
        case INTEGRITY_CHECK_HASH_CORRECT: 
        {
            printOk();
            printf("File integrity check successfull.\n\n");
            break;
        }

        case INTEGRITY_CHECK_HASH_INCORRECT: 
        {
            printFailed();
            printf("Hashes don't match.\n\n");
            return -1;
        }

        case INTEGRITY_CHECK_REF_HASH_FILE_ERROR: 
        {
            printFailed();
            printf("Error reading the file with correct hash (\"%s\").\n\n", REF_HASH_FILENAME);
            return -1;
        }

        case INTEGRITY_CHECK_FILE_ERROR: 
        {
            printFailed();
            printf("Error reading the executable (\"%s\").\n\n", patcher->filename);
            return -1;
        }

        default:
        {
            return -1;
        }
    }

    return 0;
}

void patchExecutable(Patcher* patcher)
{
    assert(patcher);

    printProgressBar("Patching...", SLEEP_LONG);
    patch(patcher);
    printOk();
    printf("Patching has finished.\n\n");
}

void saveExecutable(Patcher* patcher)
{
    assert(patcher);
    assert(patcher->buffer);

    printProgressBar("Saving...", SLEEP_SHORT);

    fwrite(patcher->buffer, patcher->bufferSize, sizeof(unsigned char), patcher->file);
    fclose(patcher->file);

    printOk();
    printf("File \"%s\" has been saved.\n\n", patcher->filename);
}

void printExit(bool successfull)
{
    if (successfull)
    {
        printOk();
        printf("File has been successfully patched!\n");
    }
    else 
    {
        printFailed();
        printf("Errors occurred during patching!\n");
    }
}

void printProgressBar(const char* message, useconds_t sleepMilliseconds)
{
    assert(message);

    for (size_t curPercentage = 0; curPercentage <= 100; curPercentage += 4)
    {
        printf("\r[");

        setFgrColor(FGR_CYAN);

        for (size_t i = 0; i < (curPercentage / 4); i++)
        {
            putchar('#');
        }

        for (size_t i = curPercentage / 4; i < PROGRESS_BAR_LENGTH; i++)
        {
            putchar(' ');
        }

        resetColor();

        printf("] [%lu%%]  ", curPercentage);

        if (message != nullptr)
        {
            setModColor(MOD_ITALIC);
            printf(message);
            resetColor();
        }

        fflush(stdout);

        usleep(sleepMilliseconds);
    }

    printf("\n");
}

void printStatusBox(const char* text, size_t foregroundColor)
{
    assert(text);

    putchar('[');
    setFgrColor(foregroundColor);
    printf(text);
    resetColor();
    printf("] ");
}

void printOk()
{
    printStatusBox("OK", FGR_GREEN);
}

void printFailed()
{
    printStatusBox("FAILED", FGR_RED);
}