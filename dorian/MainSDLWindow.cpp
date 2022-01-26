#include "MainSDLWindow.hpp"
#include <SDL2/SDL_ttf.h>

MainSDLWindow::MainSDLWindow(void){
    this->renderer = NULL;
    this->window = NULL;
}

MainSDLWindow::~MainSDLWindow(void){
    SDL_DestroyRenderer(this->renderer); 
    SDL_DestroyWindow(this->window);
    TTF_Quit();
    SDL_Quit();
}

int MainSDLWindow::Init(const char* window_name, int width, int height){
<<<<<<< HEAD
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
=======
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
>>>>>>> 4a715834201423c2ab5d5923302f7de2d2f55429
       printf("Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
       return EXIT_FAILURE;
    }

    if(TTF_Init()==-1) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
        return EXIT_FAILURE;
    }
    
    TTF_Font* police = TTF_OpenFont("/usr/share/fonts/truetype/lato/Lato-Light.ttf", 30);
    if(!police){
        printf("font-init : %s\n", TTF_GetError());
        // handle error
    }

    window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Erreur lors de la creation d'une fenetre : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

<<<<<<< HEAD
    this->renderer = SDL_CreateRenderer(this->window, -1 , SDL_RENDERER_PRESENTVSYNC);
=======
    renderer = SDL_CreateRenderer(window, -1 , SDL_RENDERER_PRESENTVSYNC);
>>>>>>> 4a715834201423c2ab5d5923302f7de2d2f55429
    if(renderer == NULL) {
        printf("Erreur lors de la creation d'un renderer : %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }

    scoreZone = {0, 0, width, ((height/10)*1)};
    playground = {0, ((height/10)*1), width, ((height/10)*9)};

    return EXIT_SUCCESS;
}

SDL_Renderer* MainSDLWindow::GetRenderer(void){
    return renderer;
}

int MainSDLWindow::GetWindowWidth(void){
    int width;
    SDL_GetWindowSize(window, &width, NULL);
    return width;
}

int MainSDLWindow::GetWindowHeight(void){
    int height;
    SDL_GetWindowSize(window, NULL, &height);
    return height;
}

SDL_Rect MainSDLWindow::GetPlaygroundZone(void){
    return playground;
}

SDL_Rect MainSDLWindow::GetScoreZone(void){
    return scoreZone;
}