#include "mplay.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define NUM_SQUARES 1000
#define RANDOM_COLOR rand() % 255

typedef struct {

    SDL_Rect rect;
    SDL_Color backgroundColor;

} Square;


bool loop(Application *app) {



    Square squares[NUM_SQUARES] = {0};
    for(size_t k = 0; k < NUM_SQUARES; k++){
        const i32 squaresize = rand() % 7;
        squares[k].rect.x = rand() % WINDOW_WIDTH;
        squares[k].rect.y = rand() % WINDOW_HEIGHT;
        squares[k].rect.h = squaresize;
        squares[k].rect.w = squaresize;
        squares[k].backgroundColor.r = RANDOM_COLOR;
        squares[k].backgroundColor.g = RANDOM_COLOR;
        squares[k].backgroundColor.b = RANDOM_COLOR;
        squares[k].backgroundColor.a = RANDOM_COLOR;

    }

    bool running = true;
    while(running){
        while (SDL_PollEvent(&app->event)) {
            switch (app->event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    if(app->event.key.keysym.sym == SDLK_ESCAPE){
                        running = false;
                        break;
                    }
                    if(app->event.key.keysym.sym == SDLK_SPACE){
                        if(Mix_PausedMusic() == 1) {
                            snprintf(app->textContext->messageBuffer, app->textContext->bufferSize, "Playing!"/*, app->event.drop.file*/);
                            Mix_ResumeMusic();
                        }
                        else {
                            snprintf(app->textContext->messageBuffer, app->textContext->bufferSize, "Paused"/*, app->event.drop.file*/);
                            Mix_PauseMusic();
                        }
                    }
                    break;
                case SDL_DROPFILE: {
                    
                    bool musicLoaded = load_music(app, app->event.drop.file, true);
                    if(musicLoaded){
                        free(app->backgroundColor);
                        app->backgroundColor = new_color(RANDOM_COLOR, RANDOM_COLOR, RANDOM_COLOR, RANDOM_COLOR);
                        snprintf(app->textContext->messageBuffer, app->textContext->bufferSize, "Playing!"/*, app->event.drop.file*/);
                        printf("Music loaded, now playing %s\n", app->event.drop.file);
                    }
                    break;

                }
                default:
                    break;
            }        
        }
        
        SDL_RenderCopy(app->renderer, app->textContext->text, NULL, NULL);
        SDL_SetRenderDrawColor(app->renderer, app->backgroundColor->r, app->backgroundColor->g, app->backgroundColor->b, app->backgroundColor->a);
        SDL_RenderClear(app->renderer);

        draw_text(app, true, app->textContext->messageBuffer, app->textColor);
        for(size_t k = 0; k < NUM_SQUARES; k++){
            squares[k].rect.y = squares[k].rect.y <= WINDOW_HEIGHT ? squares[k].rect.y + 2 : 0; 
            SDL_SetRenderDrawColor(app->renderer, squares[k].backgroundColor.r, squares[k].backgroundColor.g, squares[k].backgroundColor.b, squares[k].backgroundColor.a++); //background color
            SDL_RenderFillRect(app->renderer, &squares[k].rect);        
        }
        SDL_RenderPresent(app->renderer);
        SDL_Delay(4);

    }

    return true;
}

int main(int argc, char **argv){
    srand(time(NULL));
    Application app = {0};    
    
    app.backgroundColor = new_color(0, 0, 255, 255);
    app.textContext = new_font(FONT_PATH);

    init_elise(&app, false);
    loop(&app);
    release(&app);
    return 0;

}   

