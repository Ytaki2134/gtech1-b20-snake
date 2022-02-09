#ifndef MainSDLWindow_HPP
#define MainSDLWindow_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class ScoreRenderer;
class PlaygroundRenderer;
class Snake;
class Playground;
class Score;

class MainSDLWindow{
    public:
        MainSDLWindow(void);

        ~MainSDLWindow(void);

        int Init(const char* window_name, int width, int height);

        void Run();

    private:
        bool HandleMainLoopEvents();

        bool WaitForPlayerReady();
        bool ShowGameOver();

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Rect scoreZone;
        SDL_Rect playgroundZone;
        TTF_Font* outsideMainLoopFont;
        TTF_Font* outsideMainLoopSmallFont;
        TTF_Font* pixelizedFont;

        ScoreRenderer* scoreGraphics;
        PlaygroundRenderer* playgroundGraphics;

        Snake* mainSnake;
        Playground* playground;
        Score* score;
};

#endif