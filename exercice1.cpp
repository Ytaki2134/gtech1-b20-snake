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

void MainSDLWindow::MainLoop(Uint32 framerate){
    SDL_Event event;
    SDL_Rect snake_head = {300, 300, 50, 50}; 
    while (true){
        Uint32 frame_time_start = SDL_GetTicks();

        SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
        SDL_RenderClear(this->renderer);
        
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
                return;
            if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.scancode == SDL_SCANCODE_DOWN){
                    snake_head.y += 20; 
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_UP){
                    snake_head.y -= 20; 
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                    snake_head.x += 20; 
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_LEFT){
                    snake_head.x -= 20; 
                }
            }
        }

        SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(this->renderer, &snake_head);
        SDL_RenderPresent(this->renderer);
        
        Uint32 time_since_frame_start = SDL_GetTicks() - frame_time_start;
        if (time_since_frame_start < framerate){
            SDL_Delay(framerate - time_since_frame_start);
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
    Uint32 frame_rate = 20;

    MainSDLWindow main_window;
    main_window.Init("Snake", 600, 600);
    main_window.MainLoop(frame_rate);
    return 0;
}