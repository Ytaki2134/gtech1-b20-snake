#include "exercice1.hpp"

MainSDLWindow::MainSDLWindow(void){
    this->renderer = NULL;
    this->window = NULL;
}

MainSDLWindow::~MainSDLWindow(void){
    SDL_DestroyRenderer(this->renderer); 
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

void MainSDLWindow::WaitForClosing(){
    SDL_Event event;
    while (true){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
                return;
        }   
    }
    
}

int MainSDLWindow::Init(const char* window_name, int width, int height){
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
       printf("Erreur d'initialisation de la SDL : %s", SDL_GetError());
       return EXIT_FAILURE;
    }

    this->window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        printf("Erreur lors de la creation d'une fenetre : %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    this->renderer = SDL_CreateRenderer(this->window, -1 , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) {
        printf("Erreur lors de la creation d'un renderer : %s",SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    return EXIT_SUCCESS;
}

SDL_Renderer* MainSDLWindow::GetRenderer(void){
    return this->renderer;
}

int main(void){
    

    MainSDLWindow main_window;
    main_window.Init("test", 600, 600);
    main_window.WaitForClosing();
}