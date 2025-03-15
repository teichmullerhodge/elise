#ifndef ELISE_H
#define ELISE_H

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <string.h>
#include "helpers.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define VERSION 1.0
#define APP_TITLE "ELISE"

// sample paths for development purposes
#define SAMPLE_SONG_PATH "assets/song.wav"
#define FONT_PATH "assets/open_sans.ttf"

typedef struct {
    
    TTF_Font* font;
    SDL_Rect fontRect;
    SDL_Surface* textSurface;   
    SDL_Texture* text;
    const char *fontPath;
    char messageBuffer[1024]; // max size of the message buffer.
    i16 bufferSize;

} TextDisplay;

typedef struct {

    SDL_Window* window; 
    SDL_Renderer* renderer;
    SDL_Event event;
    SDL_Color *backgroundColor;
    Mix_Music* music;
    SDL_DropEvent *dropFile;
    TextDisplay* textContext;
    SDL_Color textColor;



} Application;


static inline SDL_Color* new_color(u8 r, u8 g, u8 b, u8 a){

    SDL_Color *c = (SDL_Color*)malloc(sizeof(SDL_Color));
    c->r = r;
    c->g = g;
    c->b = b;
    c->a = a;
    return c;


}


static inline TextDisplay* new_font(const char *path){

    TextDisplay *f = (TextDisplay*)malloc(sizeof(TextDisplay));
    f->fontPath = path;
    return f;


}


static inline void default_message(Application* app){
    snprintf(app->textContext->messageBuffer, app->textContext->bufferSize, "Drop a music file! ");
}

static inline u8 init_elise(Application *app, bool excep){

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        panic("Error initializing SDL\n", -1, excep);
        return -1;
        
    }

    if (Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024 ) < 0 ) {
        panic("Error initializing SDL_Mixer!\n", -2, excep);
        return -2;
	}

    if ( TTF_Init() < 0 ) {
        panic("Error initializing SDL_font!\n", -3, excep);
        return -3;
    }
	
    app->window = SDL_CreateWindow(APP_TITLE, SDL_WINDOWPOS_CENTERED, 
                                SDL_WINDOWPOS_CENTERED, 
                                WINDOW_WIDTH, 
                                WINDOW_HEIGHT, 0);


    if(!app->window){
        panic("Window could not be initialized!\n", -4, excep);
        return -4;

    }
        
    // SDL_SetWindowBordered(app->window, SDL_FALSE);
    app->renderer = SDL_CreateRenderer(app->window, -1, 0);
    if(!app->renderer){
        panic("Error creating the renderer!\n", -5, excep);
        return -5;
    } 

    app->textContext->font = TTF_OpenFont(app->textContext->fontPath, 16);
    if(!app->textContext->font){
        panic("Error loading the font!\n", -6, excep);
        return -6;
    }

	SDL_StartTextInput();    
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);    
    SDL_Color textColor = {0, 0, 0, 0};
    app->textContext->bufferSize = 1024;
    app->textColor = textColor;
    default_message(app);
    return 0;
}

static inline void release(Application *app){
    SDL_StopTextInput();
    TTF_CloseFont( app->textContext->font );
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    Mix_FreeMusic(app->music);
    free(app->backgroundColor);
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();

}

static inline bool load_music(Application *app, const char *path, bool play) {

    app->music = Mix_LoadMUS(path);
    if(!app->music){
        panic("Error loading the provided song!", -7, false);
        return false;
    }

    if(play) {
        Mix_PlayMusic(app->music, -1);
    }
    return true;

}

static inline void draw_text(Application* app, bool useBuffer, const char *message, SDL_Color color){
    if(!useBuffer || message != NULL){

        for(size_t k = 0; k < strlen(message); k++){
            if(message[k] == '\0') break;
            app->textContext->messageBuffer[k] = message[k];
            
        }
    
    }


    app->textContext->textSurface = TTF_RenderText_Solid(app->textContext->font, app->textContext->messageBuffer, color);
    app->textContext->text = SDL_CreateTextureFromSurface(app->renderer,app->textContext->textSurface);
    app->textContext->fontRect.x = 100;
    app->textContext->fontRect.y = 200;
    app->textContext->fontRect.w = 600; //18
    app->textContext->fontRect.h = 80;
    SDL_RenderCopy(app->renderer, app->textContext->text, NULL, &app->textContext->fontRect);
	SDL_DestroyTexture(app->textContext->text);
	SDL_FreeSurface(app->textContext->textSurface);

}

#endif // !ELISE_H
