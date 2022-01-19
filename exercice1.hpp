#include <SDL2/SDL.h>

class MainSDLWindow{
    public:
        MainSDLWindow(void);

        ~MainSDLWindow(void);

        int Init(const char* window_name, int width, int height);

        void WaitForClosing();

        SDL_Renderer* GetRenderer(void);

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
};