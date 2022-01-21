#include "MainSDLWindow.hpp"

int main(int argc, char* argv[]){
    SDL_Event event;
    SDL_Rect snake_head = {300, 300, 50, 50};

    Uint32 frame_rate = 20;

    MainSDLWindow* main_window = new MainSDLWindow();
    main_window.Init("Snake", 600, 600);
    SDL_Renderer* main_window_renderer = main_window->GetRenderer();

    while (true){
        Uint32 frame_time_start = SDL_GetTicks();

        SDL_SetRenderDrawColor(main_window_renderer, 0, 0, 0, 0);
        SDL_RenderClear(main_window_renderer);
        
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE){
                delete main_window;
                return 0;
            }
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

        SDL_SetRenderDrawColor(main_window_renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(main_window_renderer, &snake_head);
        SDL_RenderPresent(main_window_renderer);
        
        Uint32 time_since_frame_start = SDL_GetTicks() - frame_time_start;
        if (time_since_frame_start < framerate){
            SDL_Delay(framerate - time_since_frame_start);
        }
    }

    return 0;
}