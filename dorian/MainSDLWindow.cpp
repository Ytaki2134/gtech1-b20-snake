#include "MainSDLWindow.hpp"

MainSDLWindow::MainSDLWindow(void){
    this->renderer = NULL;
    this->window = NULL;
}

MainSDLWindow::~MainSDLWindow(void){
    SDL_DestroyRenderer(this->renderer); 
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

int MainSDLWindow::Init(const char* window_name, int width, int height){
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
       printf("Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
       return EXIT_FAILURE;
    }

    this->window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        printf("Erreur lors de la creation d'une fenetre : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    this->renderer = SDL_CreateRenderer(this->window, -1 , SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) {
        printf("Erreur lors de la creation d'un renderer : %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

SDL_Renderer* MainSDLWindow::GetRenderer(void){
    return this->renderer;
}