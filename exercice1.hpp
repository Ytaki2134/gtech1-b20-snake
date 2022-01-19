#include <SDL2/SDL.h>

class MainSDLWindow{
    public:

        MainSDLWindow(void){
            this->renderer = NULL;
            this->window = NULL;
        }

        ~MainSDLWindow(void){
            delete this->renderer;
            delete this->window;
        }

        int Init(const char* window_name, int width, int height){
            this->window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);

            this->renderer = SDL_CreateRenderer(this->window, -1 , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


            return EXIT_SUCCESS;
        }

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
};