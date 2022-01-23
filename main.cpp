#include "MainSDLWindow.hpp"
#include "Snake.hpp"

int main(void){
    Uint32 framerate = 20;
    int windowSize = 600;

    MainSDLWindow* mainWindow = new MainSDLWindow();
    mainWindow->Init("Snake", 600, 600);

    SDL_Renderer* mainWindowRenderer = mainWindow->GetRenderer();
    SDL_Event event;
    Snake* mainSnake = new Snake(25, 25, RIGHT, 10);
    while (true){
        Uint32 frameTimeStart = SDL_GetTicks();

        SDL_SetRenderDrawColor(mainWindowRenderer, 0, 0, 0, 0);
        SDL_RenderClear(mainWindowRenderer);
        
        mainSnake->Move();

        while (SDL_PollEvent(&event)){
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
                return 0;
            if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.scancode == SDL_SCANCODE_DOWN){
                    mainSnake->ChangeDirection(DOWN);
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_UP){
                    mainSnake->ChangeDirection(UP); 
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                    mainSnake->ChangeDirection(RIGHT);
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT){
                    mainSnake->ChangeDirection(LEFT);
                }
            }
        }
        
        mainSnake->draw(mainWindowRenderer, windowSize/100);
        SDL_RenderPresent(mainWindowRenderer);
        
        Uint32 timeSinceFrameStart = SDL_GetTicks() - frameTimeStart;
        if (timeSinceFrameStart < framerate){
            SDL_Delay(framerate - timeSinceFrameStart);
        }
    }

    delete mainWindow;
    return 0;
}