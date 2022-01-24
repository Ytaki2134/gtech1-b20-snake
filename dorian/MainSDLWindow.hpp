#ifndef MainSDLWindow_HPP
#define MainSDLWindow_HPP

#include <SDL2/SDL.h>

class MainSDLWindow{
    public:
        MainSDLWindow(void);

        ~MainSDLWindow(void);

        int Init(const char* window_name, int width, int height);

        SDL_Renderer* GetRenderer(void);
        SDL_Rect* GetPlayground(void);

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Rect scoreZone;
        SDL_Rect playground;
};

#endif