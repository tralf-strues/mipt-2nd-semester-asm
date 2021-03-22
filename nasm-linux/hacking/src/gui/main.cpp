#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "gui_handler.h"
#include "texture.h"
#include "progress_bar.h"
#include "button.h"
#include "../patcher.h"

const int      SCREEN_WIDTH                  = 800;  
const int      SCREEN_HEIGHT                 = 600;
const char*    SCREEN_TITLE                  = "Patcher";

const char*    FONT_FILENAME                 = "res/SF Atarian System Bold.ttf";
const size_t   FONT_SIZE                     = 16;

const uint32_t BACKGROUND_COLOR              = 0x203A40FF;
const uint32_t UI_COLOR                      = 0xEEF145FF;
const uint32_t ERROR_COLOR                   = 0x9D2C00FF;

const size_t   SKULL_TEXTURE_WIDTH           = 300;
const size_t   SKULL_TEXTURE_HEIGHT          = 200;
const size_t   SKULL_POSITION_X              = SCREEN_WIDTH / 2 - SKULL_TEXTURE_WIDTH / 2;
const size_t   SKULL_POSITION_Y              = 20;
const char*    SKULL_TEXTURE_FILENAME        = "res/skull.png";

const size_t   PROGRESS_BAR_WIDTH            = 300;
const size_t   PROGRESS_BAR_HEIGHT           = 26;
const size_t   PROGRESS_BAR_MARGIN           = 4;
const uint32_t PROGRESS_BAR_COLOR_BORDER     = UI_COLOR;
const uint32_t PROGRESS_BAR_COLOR_PERCENTAGE = 0x009D90FF;

const size_t   PROGRESS_BAR_OPENING_SLEEP    = 20000;
const size_t   PROGRESS_BAR_OPENING_Y        = 350;
const size_t   PROGRESS_BAR_INTEGRITY_SLEEP  = 30000;
const size_t   PROGRESS_BAR_INTEGRITY_Y      = 400;
const size_t   PROGRESS_BAR_PATCHING_SLEEP   = 50000;
const size_t   PROGRESS_BAR_PATCHING_Y       = 450;
const size_t   PROGRESS_BAR_SAVING_SLEEP     = 20000;
const size_t   PROGRESS_BAR_SAVING_Y         = 500;

const size_t   BUTTON_WIDTH                  = 64;
const size_t   BUTTON_HEIGHT                 = 64;
const size_t   BUTTON_X                      = SCREEN_WIDTH  / 2 - BUTTON_WIDTH  / 2;
const size_t   BUTTON_Y                      = 1.5 * SKULL_POSITION_Y + SKULL_TEXTURE_HEIGHT;
const char*    BUTTON_ICON_FILENAME          = "res/open_folder_icon.png";

const size_t   MAX_FILENAME_LENGTH           = 256;

const char*    FILE_OPENING_TEXT             = "Opening the file...";
const char*    FILE_OPENING_FAILED_TEXT      = "File opening failed!";
const char*    FILE_OPENING_FINISHED_TEXT    = "File opened";

const char*    FILE_INTEGRITY_TEXT           = "Checking file integrity...";
const char*    FILE_INTEGRITY_FAILED_TEXT    = "File integrity check failed!";
const char*    FILE_INTEGRITY_FINISHED_TEXT  = "File correct";

const char*    FILE_PATCHING_TEXT            = "Patching...";
const char*    FILE_PATCHING_FAILED_TEXT     = "Patching failed!";
const char*    FILE_PATCHING_FINISHED_TEXT   = "Patched";

const char*    FILE_SAVING_TEXT              = "Saving...";
const char*    FILE_SAVING_FAILED_TEXT       = "Saving failed!";
const char*    FILE_SAVING_FINISHED_TEXT     = "Saved";

enum ProgramState
{
    INTRO, 
    FILE_SELECTED,

    FILE_OPENING,
    FILE_OPENING_ERROR,

    FILE_CHECKING_INTEGRITY,
    FILE_INTEGRITY_ERROR,

    FILE_PATCHING,
    FILE_PATCHING_ERROR,

    FILE_SAVING,
    FILE_SAVING_ERROR,

    FINISHED
};

struct GuiData
{
    Button*      openFileButton;
    ProgressBar* openFileBar;
    ProgressBar* integrityFileBar;
    ProgressBar* patchingBar;
    ProgressBar* savingBar;

    Texture*     skullTexture;
    Texture*     textTexturePercentage;
    Texture*     textTextureOpeningFile;
    Texture*     textTextureIntegrityFile;
    Texture*     textTexturePatching;
    Texture*     textTextureSaving;

    char*        filename;
    ProgramState curState;
    float        curStatePercantage;
};

void runGui              (GuiHandler* handler);
void update              (GuiHandler* handler, void* data);
void processEvent        (GuiHandler* handler, const SDL_Event* event, void* data);
bool isCursorOnButton    (const Button* button, int cursorX, int cursorY);
void onClick             (void* data);
void initProgressBar     (ProgressBar* bar, size_t y, TTF_Font* font, Texture* texture);
void processProgressBar  (GuiHandler* handler, const GuiData* guiData, ProgramState state,
                          ProgressBar* bar,
                          const char* textProgress, 
                          const char* textFailed, 
                          const char* textFinished);
void  mimicProccessGoing (GuiData* guiData, size_t finalPercentage, 
                          ProgramState start, ProgramState finish,
                          size_t microsecondsPerPercent);            
void* runPatching        (void* arg);

int main()
{
    FILE* audioPipe = popen("aplay res/patcher_audio.wav &>/dev/null", "r");

    GuiHandler handler = {};

    if (!init(&handler, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE, {FONT_FILENAME, FONT_SIZE}))
    {
        printf("[ERROR] Failed to initialize!\n");
        pclose(audioPipe);
        return -1;
    }

    runGui(&handler);

    close(&handler);
    pclose(audioPipe);

    return 0;
}

void runGui(GuiHandler* handler)
{
    assert(handler);

    Texture skullTexture = {};
    loadTexture(&skullTexture, SKULL_TEXTURE_FILENAME, handler->renderer);

    // Init progress bars
    Texture textTexturePercentage    = {};
    Texture textTextureOpeningFile   = {};
    Texture textTextureIntegrityFile = {};
    Texture textTexturePatching      = {};
    Texture textTextureSaving        = {};
    Texture buttonIcon               = {};

    ProgressBar openFileBar = {};
    initProgressBar(&openFileBar, PROGRESS_BAR_OPENING_Y, handler->font, &textTexturePercentage);

    ProgressBar integrityFileBar = {};
    initProgressBar(&integrityFileBar, PROGRESS_BAR_INTEGRITY_Y, handler->font, &textTexturePercentage);

    ProgressBar patchingBar = {};
    initProgressBar(&patchingBar, PROGRESS_BAR_PATCHING_Y, handler->font, &textTexturePercentage);

    ProgressBar savingBar = {};
    initProgressBar(&savingBar, PROGRESS_BAR_SAVING_Y, handler->font, &textTexturePercentage);

    Button openFileButton = {};
    loadTexture(&buttonIcon, BUTTON_ICON_FILENAME, handler->renderer);
    createButton(handler->renderer, &openFileButton, BUTTON_X, BUTTON_Y, &buttonIcon);
    setOnClick(&openFileButton, onClick);

    char filename[MAX_FILENAME_LENGTH] = {};

    GuiData guiData = {&openFileButton, 
                       &openFileBar,
                       &integrityFileBar,
                       &patchingBar,
                       &savingBar,
                       &skullTexture,
                       &textTexturePercentage,
                       &textTextureOpeningFile,
                       &textTextureIntegrityFile,
                       &textTexturePatching,
                       &textTextureSaving,
                       filename,
                       INTRO,
                       0};

    run(handler, &guiData, processEvent, update);

    destroyButton(&openFileButton);
    destroyTexture(&skullTexture);
    destroyTexture(&textTexturePercentage);
    destroyTexture(&textTextureOpeningFile);
    destroyTexture(&textTextureIntegrityFile);
    destroyTexture(&textTexturePatching);
    destroyTexture(&textTextureSaving);
}

void update(GuiHandler* handler, void* data)
{
    assert(handler);
    assert(data);

    GuiData* guiData = (GuiData*) data; 

    SDL_SetRenderDrawColor(handler->renderer, 
                           getR(BACKGROUND_COLOR), 
                           getG(BACKGROUND_COLOR), 
                           getB(BACKGROUND_COLOR), 
                           getA(BACKGROUND_COLOR));
    SDL_RenderClear(handler->renderer);

    renderTexture(guiData->skullTexture, 
                  SKULL_POSITION_X,    SKULL_POSITION_Y, 
                  SKULL_TEXTURE_WIDTH, SKULL_TEXTURE_HEIGHT, 
                  handler->renderer);

    renderButton(handler->renderer, guiData->openFileButton);

    processProgressBar(handler, guiData, FILE_OPENING, 
                       guiData->openFileBar, 
                       FILE_OPENING_TEXT,
                       FILE_OPENING_FAILED_TEXT,
                       FILE_OPENING_FINISHED_TEXT);

    processProgressBar(handler, guiData, FILE_CHECKING_INTEGRITY, 
                       guiData->integrityFileBar, 
                       FILE_INTEGRITY_TEXT,
                       FILE_INTEGRITY_FAILED_TEXT,
                       FILE_INTEGRITY_FINISHED_TEXT);

    processProgressBar(handler, guiData, FILE_PATCHING, 
                       guiData->patchingBar, 
                       FILE_PATCHING_TEXT,
                       FILE_PATCHING_FAILED_TEXT,
                       FILE_PATCHING_FINISHED_TEXT);

    processProgressBar(handler, guiData, FILE_SAVING, 
                       guiData->savingBar, 
                       FILE_SAVING_TEXT,
                       FILE_SAVING_FAILED_TEXT,
                       FILE_SAVING_FINISHED_TEXT);

    SDL_RenderPresent(handler->renderer);
}

void processEvent(GuiHandler* handler, const SDL_Event* event, void* data)
{
    assert(handler);
    assert(event);
    assert(data);

    GuiData* guiData = (GuiData*) data;

    switch (event->type)
    {
        case SDL_QUIT:
        {
            handler->running = false;
            break;
        }

        case SDL_MOUSEBUTTONDOWN:
        {
            if (isCursorOnButton(guiData->openFileButton, event->motion.x, event->motion.y))
            {
                guiData->openFileButton->onClick(guiData);
            }

            break;
        }

        case SDL_KEYDOWN:
        {
            if (event->key.keysym.scancode == SDL_SCANCODE_5)
            {
                guiData->curState = (ProgramState) (guiData->curState + 1);
            }
            else if (event->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                handler->running = false;
            }

            break;
        }
        
        default: { break; }
    }
}

bool isCursorOnButton(const Button* button, int cursorX, int cursorY)
{
    assert(button);

    printf("Cursor = (%d, %d); ",   cursorX,   cursorY);
    printf("Button = (%lu, %lu)\n", button->x, button->y);

    return (cursorX >= (int) button->x) && (cursorX <= (int) (button->x + button->icon->width)) &&
           (cursorY >= (int) button->y) && (cursorY <= (int) (button->y + button->icon->height));
}

void onClick(void* data)
{
    assert(data);

    GuiData* guiData = (GuiData*) data; 

    if (guiData->curState == INTRO)
    {
        FILE* file = popen("zenity --file-selection", "r");
        fgets(guiData->filename, MAX_FILENAME_LENGTH, file);
        guiData->filename[strlen(guiData->filename) - 1] = 0;
        pclose(file);

        guiData->curState = FILE_SELECTED;  

        pthread_t patchingThread = {};
        pthread_create(&patchingThread, nullptr, runPatching, guiData);   
    }
}

void initProgressBar(ProgressBar* bar, size_t y, TTF_Font* font, Texture* texture)
{
    assert(bar);
    assert(y < SCREEN_HEIGHT);
    assert(font);
    assert(texture);

    bar->percentage      = 0;
    bar->x               = (SCREEN_WIDTH / 2) - (PROGRESS_BAR_WIDTH / 2);
    bar->y               = y;
    bar->width           = PROGRESS_BAR_WIDTH;
    bar->height          = PROGRESS_BAR_HEIGHT;
    bar->margin          = PROGRESS_BAR_MARGIN;
    bar->colorBorder     = PROGRESS_BAR_COLOR_BORDER;
    bar->colorPercentage = PROGRESS_BAR_COLOR_PERCENTAGE;
    bar->font            = font;
    bar->texture         = texture;
}

void processProgressBar(GuiHandler* handler, const GuiData* guiData, ProgramState state,
                        ProgressBar* bar,
                        const char* textProgress, 
                        const char* textFailed, 
                        const char* textFinished)
{
    assert(handler);
    assert(handler->renderer);
    assert(guiData);
    assert(bar);
    assert(textProgress);
    assert(textFailed);
    assert(textFinished);

    if (guiData->curState >= state)
    {
        bar->colorPercentage = PROGRESS_BAR_COLOR_PERCENTAGE;
        bar->percentage      = guiData->curStatePercantage;
        
        if (guiData->curState == state)
        {
            bar->text = textProgress; 
        }
        else if (guiData->curState == state + 1) // error state goes right after state
        {
            bar->text            = textFailed;
            bar->colorPercentage = ERROR_COLOR;
        }
        else 
        {
            bar->text       = textFinished; 
            bar->percentage = 1;
        }

        renderProgressBar(handler->renderer, bar);
    }
}

void mimicProccessGoing(GuiData* guiData, size_t finalPercentage, 
                        ProgramState start, ProgramState finish,
                        size_t microsecondsPerPercent)
{
    assert(guiData);
    assert(finalPercentage <= 100);

    guiData->curState = start;

    for (size_t i = 0; i <= finalPercentage; i++)
    {
        guiData->curStatePercantage = 0.01 * i;
        usleep(microsecondsPerPercent);
    }

    guiData->curState = finish;
}

void* runPatching(void* arg)
{
    GuiData* guiData = (GuiData*) arg;

    Patcher patcher = {};   
    srand(time(nullptr));

    // Opening file
    if (init(&patcher, guiData->filename) == -1)
    {
        mimicProccessGoing(guiData, rand() % 101, 
                           FILE_OPENING, FILE_OPENING_ERROR, 
                           PROGRESS_BAR_OPENING_SLEEP);
        return nullptr;
    }
    else
    {
        mimicProccessGoing(guiData, 100, 
                           FILE_OPENING, FILE_OPENING, 
                           PROGRESS_BAR_OPENING_SLEEP);
    }

    // Checking file's integrity
    IntegrityCheckStatus status = checkExecIntegrity(&patcher);
    if (status != INTEGRITY_CHECK_HASH_CORRECT)
    {
        printf("Integrity error = %d\n", status);

        mimicProccessGoing(guiData, rand() % 101, 
                           FILE_CHECKING_INTEGRITY, FILE_INTEGRITY_ERROR, 
                           PROGRESS_BAR_INTEGRITY_SLEEP);
        return nullptr;
    }
    else
    {
        mimicProccessGoing(guiData, 100, 
                           FILE_CHECKING_INTEGRITY, FILE_CHECKING_INTEGRITY, 
                           PROGRESS_BAR_INTEGRITY_SLEEP);
    }

    // File patching
    patch(&patcher);
    mimicProccessGoing(guiData, 100, 
                       FILE_PATCHING, FILE_PATCHING, 
                       PROGRESS_BAR_PATCHING_SLEEP);

    // File saving
    fwrite(patcher.buffer, patcher.bufferSize, sizeof(char), patcher.file);
    fclose(patcher.file);
    mimicProccessGoing(guiData, 100, 
                       FILE_SAVING, FINISHED, 
                       PROGRESS_BAR_SAVING_SLEEP);

    return nullptr;
}