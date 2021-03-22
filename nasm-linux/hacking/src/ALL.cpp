#pragma once

#include <stdio.h>

unsigned char* readBufferFromFile (FILE* file, size_t* bufferSize);
size_t         getFileSize        (FILE* file); 

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "file_loader.h"

unsigned char* readBufferFromFile(FILE* file, size_t* bufferSize)
{
    assert(file);
    assert(bufferSize);

    *bufferSize = getFileSize(file);
    unsigned char* buffer = (unsigned char*) calloc(*bufferSize, sizeof(unsigned char));
    assert(buffer);

    fread(buffer, sizeof(unsigned char),  *bufferSize, file);

    return buffer;
}

size_t getFileSize(FILE* file)
{
    assert(file);

    struct stat fileInfo = {};
    fstat(fileno(file), &fileInfo);
    
    return fileInfo.st_size;
}

#include "hash.h"

int writeHashToFile(FILE* file, uint32_t hash)
{
    if (file == nullptr)                               { return -1; }
    if (fwrite(&hash, sizeof(uint32_t), 1, file) == 0) { return -1; }

    return 0;
}

int readHashFromFile(FILE* file, uint32_t* hash)
{
    assert(hash);

    if (file == nullptr) { return -1; }

    uint32_t hashFromFile = 0;
    if (fread(&hashFromFile, 1, sizeof(uint32_t), file) == 0)
    {
        return -1;
    }

    *hash = hashFromFile;

    return 0;
}

uint32_t getHashCrc32(const unsigned char* buffer, size_t bytesCount)
{   
    assert(buffer);
    assert(bytesCount > 0);

    unsigned int hash = 0;    
    for (size_t i = 0; i < bytesCount; i++)
    {
        hash = (hash << 8) ^ CRC32_TABLE[((hash >> 24) ^ buffer[i]) & 255];
    }

    return hash;        
}

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "patcher.h"
#include "hash.h"
#include "file_loader.h"

int init(Patcher* patcher, const char* filename)
{
    assert(patcher);
    assert(filename);

    FILE* file = fopen(filename, "r");
    if (file != nullptr)
    {
        patcher->file     = file;
        patcher->filename = filename;
    }

    return file == nullptr ? -1 : 0;
}

void destroy(Patcher* patcher)
{
    assert(patcher);

    patcher->file     = nullptr;
    patcher->filename = nullptr;

    fclose(patcher->file);
}

IntegrityCheckStatus checkExecIntegrity(Patcher* patcher)
{
    assert(patcher);

    FILE* refHashFile = fopen(REF_HASH_FILENAME, "r");

    uint32_t correctHash = 0;
    if (refHashFile == nullptr || readHashFromFile(refHashFile, &correctHash) == -1)
    {
        return INTEGRITY_CHECK_REF_HASH_FILE_ERROR;
    }     

    size_t bufferSize = 0;
    unsigned char* buffer = readBufferFromFile(patcher->file, &bufferSize);

    if (buffer == nullptr)
    {
        return INTEGRITY_CHECK_FILE_ERROR;
    }

    patcher->buffer     = buffer;
    patcher->bufferSize = bufferSize;

    fclose(patcher->file);
    patcher->file = fopen(PATCHED_EXEC_FILENAME, "w");

    if (patcher->file == nullptr)
    {
        return INTEGRITY_CHECK_FILE_ERROR;
    }

    return getHashCrc32(buffer, bufferSize) == correctHash ? INTEGRITY_CHECK_HASH_CORRECT : 
                                                             INTEGRITY_CHECK_HASH_INCORRECT;
}

void patch(Patcher* patcher)
{
    assert(patcher);
    assert(patcher->buffer);

    patcher->buffer[BYTE_TO_CHANGE_POS] = BYTE_TO_CHANGE_RES;
}

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

#include <assert.h>
#include <stdio.h>

#include "file_loader.h"
#include "hash.h"

const char* DEFAULT_OUTPUT_FILENAME = "a.hash";

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        printf("No file to compute the hash of!\n");
        return -1;
    }

    FILE* inputFile = fopen(argv[1], "r");
    assert(inputFile);

    FILE* outputFile = fopen(argc > 2 ? argv[2] : DEFAULT_OUTPUT_FILENAME, "w");
    assert(inputFile);

    size_t         bufferSize = 0;
    unsigned char* buffer     = readBufferFromFile(inputFile, &bufferSize);
    assert(buffer != nullptr);

    uint32_t hashValue = getHashCrc32(buffer, bufferSize);
    writeHashToFile(outputFile, hashValue);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

#include "colored_output.h"

void setColor(size_t modifier, size_t foreground, size_t background)
{
    printf("\e[%lu;%lu;%lum", modifier, foreground, background);
}

void setModColor(size_t modifier)
{
    printf("\e[%lum", modifier);
}

void setFgrColor(size_t foreground)
{
    printf("\e[%lum", foreground);
}

void setBgrColor(size_t background)
{
    printf("\e[%lum", background);
}

void resetColor()
{
    printf("\e[0m\e[0m\e[0m");
}

#pragma once

#include <stdio.h>

static const size_t MOD_BRIGHT    = 1;
static const size_t MOD_ITALIC    = 3;
static const size_t MOD_UNDERLINE = 4;
static const size_t MOD_FLASHING  = 5;

static const size_t FGR_BLACK     = 30;
static const size_t FGR_RED       = 31;
static const size_t FGR_GREEN     = 32;
static const size_t FGR_YELLOW    = 33;
static const size_t FGR_BLUE      = 34;
static const size_t FGR_PURPLE    = 35;
static const size_t FGR_CYAN      = 36;
static const size_t FGR_WHITE     = 37;

static const size_t BGR_BLACK     = 40;
static const size_t BGR_RED       = 41;
static const size_t BGR_GREEN     = 42;
static const size_t BGR_YELLOW    = 43;
static const size_t BGR_BLUE      = 44;
static const size_t BGR_PURPLE    = 45;
static const size_t BGR_CYAN      = 46;
static const size_t BGR_WHITE     = 47;

void setColor    (size_t modifier, size_t foreground, size_t background);
void setModColor (size_t modifier);
void setFgrColor (size_t foreground);
void setBgrColor (size_t background);
void resetColor  ();

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

#include <assert.h>

#include "button.h"

bool createButton(SDL_Renderer* renderer, Button* button, 
                  size_t x, size_t y, Texture* icon)
{
    assert(renderer);
    assert(button);
    assert(icon);

    button->x    = x;
    button->y    = y;
    button->icon = icon;

    return true;
}

void setOnClick(Button* button, void (*onClick) (void* data))
{
    assert(button);
    button->onClick = onClick;
}

void renderButton(SDL_Renderer* renderer, Button* button)
{
    assert(renderer);
    assert(button);
    assert(button->icon);

    renderTexture(button->icon, button->x, button->y, 
                  button->icon->width, button->icon->height,
                  renderer);
}

void destroyButton(Button* button)
{
    assert(button);

    button->x       = 0;
    button->y       = 0;
    button->onClick = nullptr;

    if (button->icon != nullptr)
    {
        destroyTexture(button->icon);
        button->icon = nullptr;
    }
}

#pragma once

#include "texture.h"

struct Button
{
    size_t   x;
    size_t   y;
    Texture* icon;
    void     (*onClick) (void* data);
};

bool createButton  (SDL_Renderer* renderer, Button* button, 
                    size_t x, size_t y, Texture* icon);
void setOnClick    (Button* button, void (*onClick) (void* data));
void renderButton  (SDL_Renderer* renderer, Button* button);
void destroyButton (Button* button);

/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render( int x, int y );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gFooTexture;
LTexture gBackgroundTexture;


LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render( int x, int y )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load Foo' texture
	if( !gFooTexture.loadFromFile( "10_color_keying/foo.png" ) )
	{
		printf( "Failed to load Foo' texture image!\n" );
		success = false;
	}
	
	//Load background texture
	if( !gBackgroundTexture.loadFromFile( "10_color_keying/background.png" ) )
	{
		printf( "Failed to load background texture image!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gFooTexture.free();
	gBackgroundTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render background texture to screen
				gBackgroundTexture.render( 0, 0 );

				//Render Foo' to the screen
				gFooTexture.render( 240, 190 );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

#include <assert.h>

#include "gui_handler.h"

bool init(GuiHandler* handler, 
          size_t screenWidth, size_t screenHeight, 
          const char* screenTitle, Font font)
{
    assert(handler);
    assert(screenTitle);

    handler->status  = NO_ERROR;
    handler->running = false;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("[ERROR] SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        
        handler->status = SDL_INIT_ERROR; 
        return false;
    }

    //Initialize PNG loading
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("[ERROR] SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        
        handler->status = SDL_IMG_INIT_ERROR; 
        return false;
    }

    if(TTF_Init() == -1)
    {
        printf("[ERROR] SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    handler->font = TTF_OpenFont(font.filename, font.size);
    if (handler->font == nullptr)
    {
        printf("[ERROR] SDL could not load fonts! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    handler->window = SDL_CreateWindow(screenTitle, 
                                       SDL_WINDOWPOS_UNDEFINED, 
                                       SDL_WINDOWPOS_UNDEFINED, 
                                       screenWidth, 
                                       screenHeight, 
                                       SDL_WINDOW_SHOWN);
    if (handler->window == nullptr)
    {
        printf("[ERROR] Window could not be created! SDL Error: %s\n", SDL_GetError());

        handler->status = WINDOW_CREATE_ERROR; 
        return false;
    }
    
    handler->renderer = SDL_CreateRenderer(handler->window, -1, SDL_RENDERER_SOFTWARE);
    if (handler->renderer == nullptr)
    {
        printf("[ERROR] Renderer could not be created! SDL Error: %s\n", SDL_GetError());

        handler->status = RENDERER_CREATE_ERROR;
        return false;
    }
    
	return true;
}

void run(GuiHandler* handler,
         void* data,
         void (*processEvent)(GuiHandler* handler, const SDL_Event* event, void* data),
         void (*update)      (GuiHandler* handler, void* data))
{
    assert(handler);
    assert(processEvent);
    assert(update);

    // TODO: add fps counter here

    handler->running = true;

    SDL_Event event = {};
    while (handler->running)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            processEvent(handler, &event, data);
        }

        update(handler, data);
    }
}

void close(GuiHandler* handler)
{
    assert(handler);

	SDL_DestroyRenderer(handler->renderer);
	SDL_DestroyWindow(handler->window);

    handler->status       = NO_ERROR;
    handler->running      = false;
	handler->window       = nullptr;
	handler->renderer     = nullptr;
	handler->font         = nullptr;
	handler->screenWidth  = 0;
	handler->screenHeight = 0;
	handler->screenTitle  = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

enum GuiError
{
    NO_ERROR,
    SDL_INIT_ERROR,
    SDL_IMG_INIT_ERROR,
    WINDOW_CREATE_ERROR,
    RENDERER_CREATE_ERROR,
    ERRORS_TOTAL
};

struct GuiHandler
{
    GuiError      status;
    bool          running;

    SDL_Window*   window;
    SDL_Renderer* renderer;
    TTF_Font*     font;

    size_t        screenWidth;
    size_t        screenHeight;
    const char*   screenTitle;
};

struct Font
{
    const char* filename;
    size_t      size;
};

bool init  (GuiHandler* handler, 
            size_t screenWidth, size_t screenHeight, 
            const char* screenTitle, Font font);

void close (GuiHandler* handler);

void run   (GuiHandler* handler,
            void* data,
            void (*processEvent)(GuiHandler* handler, const SDL_Event* event, void* data),
            void (*update)      (GuiHandler* handler, void* data));

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

#include <assert.h>

#include "progress_bar.h"

void renderProgressBar(SDL_Renderer* renderer, ProgressBar* bar)
{
    assert(renderer);
    assert(bar);

    SDL_Rect rectBorder = {bar->x, 
                           bar->y, 
                           bar->width, 
                           bar->height};

    SDL_SetRenderDrawColor(renderer, 
                           getR(bar->colorBorder), 
                           getG(bar->colorBorder), 
                           getB(bar->colorBorder), 
                           getA(bar->colorBorder));
    SDL_RenderDrawRect(renderer, &rectBorder);

    if (bar->percentage > 0)
    {
        SDL_Rect rectPercentage = {bar->x + bar->margin, 
                                   bar->y + bar->margin, 
                                   (bar->width - 2 * bar->margin) * bar->percentage, 
                                   bar->height - 2 * bar->margin};
       
        SDL_SetRenderDrawColor(renderer, 
                               getR(bar->colorPercentage), 
                               getG(bar->colorPercentage), 
                               getB(bar->colorPercentage), 
                               getA(bar->colorPercentage));  
        SDL_RenderFillRect(renderer, &rectPercentage);

        if (bar->text != nullptr && bar->texture != nullptr)
        {
            textToTexture(bar->texture, bar->text, getSDLColor(bar->colorBorder), bar->font, renderer);
         
            size_t textX = bar->x + (bar->width  / 2) - bar->texture->width  / 2;
            size_t textY = bar->y + (bar->height / 2) - bar->texture->height / 2;
            renderTexture(bar->texture, textX, textY, bar->texture->width, bar->texture->height, renderer);
        }
    }
}

#pragma once 

#include "texture.h"

struct ProgressBar
{
    float       percentage;
    size_t      x;
    size_t      y;
    size_t      width;
    size_t      height;
    size_t      margin;
    uint32_t    colorBorder;
    uint32_t    colorPercentage;
    const char* text;
    TTF_Font*   font;
    Texture*    texture;
};

void renderProgressBar(SDL_Renderer* renderer, ProgressBar* bar);

#include <assert.h>

#include "texture.h"

char getByte(uint32_t number, size_t byteNumber);

bool loadTexture(Texture* texture, const char* filename, SDL_Renderer* renderer)
{
    assert(texture);
    assert(filename);
    assert(renderer);

	texture->sdlTexture = nullptr;
    texture->width      = 0;
    texture->height     = 0;

	SDL_Surface* loadedSurface = IMG_Load(filename);
	if(loadedSurface == nullptr)
	{
		printf("[ERROR] Unable to load image \"%s\"! SDL_image Error: %s\n", 
               filename, 
               IMG_GetError());

        return false;
	}

    texture->sdlTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(texture->sdlTexture == nullptr)
    {
        printf("[ERROR] Unable to create texture from %s! SDL Error: %s\n", 
               filename, 
               SDL_GetError());

        return false;
    }

    texture->width  = loadedSurface->w;
    texture->height = loadedSurface->h;

    SDL_FreeSurface(loadedSurface);

	return true;
}

void destroyTexture(Texture* texture)
{
    assert(texture);
    
    texture->width  = 0;
    texture->height = 0;
    if (texture->sdlTexture != nullptr)
    {
        SDL_DestroyTexture(texture->sdlTexture);
    }
}

bool textToTexture(Texture* texture, 
                   const char* text, SDL_Color color, TTF_Font* font, 
                   SDL_Renderer* renderer)
{
    assert(texture);
    assert(text);
    assert(font);
    assert(renderer);

    SDL_Surface* loadedSurface = TTF_RenderText_Blended(font, text, color);
    if (loadedSurface == nullptr)
    {
        printf("[ERROR] Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    texture->sdlTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (texture->sdlTexture == nullptr)
    {
        printf("[ERROR] Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    
    texture->width  = loadedSurface->w;
    texture->height = loadedSurface->h;

    SDL_FreeSurface(loadedSurface);
    
    return true;
}

void renderTexture(Texture* texture, 
                   size_t x, size_t y, 
                   size_t width, size_t height, 
                   SDL_Renderer* renderer)
{
    assert(texture);   
    assert(renderer);

    SDL_Rect renderQuad = {x, y, width, height};
	SDL_RenderCopy(renderer, texture->sdlTexture, nullptr, &renderQuad);   
}

uint32_t getRGBA(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
    return (r << 24) + (g << 16) + (b << 8) + a;
}

SDL_Color getSDLColor(uint32_t color)
{
    return {getR(color), getG(color), getB(color), getA(color)};
}

char getByte(uint32_t number, size_t byteNumber)
{
    return (number & (0xFF << (8 * byteNumber))) >> (8 * byteNumber);
}

char getR(uint32_t color)
{
    return getByte(color, 3);
}

char getG(uint32_t color)
{
    return getByte(color, 2);
}

char getB(uint32_t color)
{
    return getByte(color, 1);
}

char getA(uint32_t color)
{
    return getByte(color, 0);
}

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

struct Texture
{
    SDL_Texture* sdlTexture;
    size_t       width;
    size_t       height;
};

bool loadTexture    (Texture* texture, const char* filename, SDL_Renderer* renderer);
void destroyTexture (Texture* texture);
bool textToTexture  (Texture* texture, const char* text, SDL_Color color, TTF_Font* font, 
                     SDL_Renderer* renderer);
void renderTexture  (Texture* texture, 
                     size_t x, size_t y, 
                     size_t width, size_t height, 
                     SDL_Renderer* renderer);

uint32_t  getRGBA     (uint32_t r, uint32_t g, uint32_t b, uint32_t a);
SDL_Color getSDLColor (uint32_t color);
char      getR        (uint32_t color);
char      getG        (uint32_t color);
char      getB        (uint32_t color);
char      getA        (uint32_t color);