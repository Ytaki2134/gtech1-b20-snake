#ifndef SCORERENDERER_HPP
#define SCORERENDERER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


class Score;

class ScoreRenderer{
    public:
        ScoreRenderer();
        ~ScoreRenderer();

        int Init(SDL_Renderer* renderer, SDL_Rect drawZone);

        void SetScore(Score* score);

        void draw();

    private:
        int InitIconsTextures();

    private:
        SDL_Rect drawZone;
        SDL_Renderer* renderer;
        SDL_Texture* iconsTextures[2];
        TTF_Font* police;

        Score* score;
};

#endif