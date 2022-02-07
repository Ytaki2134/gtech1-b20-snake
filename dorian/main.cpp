#include <algorithm>

#include "MainSDLWindow.hpp"
#include "snake.hpp"
#include "score.hpp"
#include "scorerenderer.hpp"
#include "playground.hpp"
#include "playgroundrenderer.hpp"
#include "fruits.hpp"

int main(int argc, char* argv[]){
    int framerate = 50;
    int windowWidth = 1000;
    int windowHeight = 1000;
    int nbRow = 30;
    int nbCol = 30;
    bool waitingForStart = true;
    bool gameContinues = true;
    
    MainSDLWindow* mainWindow = new MainSDLWindow();
    bool failedInit = mainWindow->Init("Snake", windowWidth, windowHeight);
    if(failedInit){
        return 1;
    }
    
    SDL_Renderer* mainWindowRenderer = mainWindow->GetRenderer();
    SDL_Event event;

    
    Snake* mainSnake = new Snake(nbRow/2, nbCol/2, RIGHT, std::min(nbRow, nbCol)/6);

    Playground* playground = new Playground(nbRow, nbCol, mainSnake);
    PlaygroundRenderer* playgroundGraphics = new PlaygroundRenderer();
    failedInit = playgroundGraphics->Init(mainWindowRenderer, mainWindow->GetPlaygroundZone(), playground);

    if(failedInit){
        return 1;
    }

    Score* score = new Score();
    ScoreRenderer* scoreGraphics = new ScoreRenderer();
    failedInit = scoreGraphics->Init(mainWindowRenderer, mainWindow->GetScoreZone());
    if(failedInit){
        return 1;
    }
    
    TTF_Font* outsideMainLoopFont = TTF_OpenFont("arial.ttf", 64);
    
    SDL_Surface* gameStartSurface = TTF_RenderText_Blended(outsideMainLoopFont, "Press to start !", {255, 255, 255, 255});
    SDL_Texture* gameStartText = SDL_CreateTextureFromSurface(mainWindowRenderer, gameStartSurface);
    SDL_FreeSurface(gameStartSurface);
    SDL_Rect gameStartPlacement;
    SDL_QueryTexture(gameStartText, NULL, NULL, &gameStartPlacement.w, &gameStartPlacement.h);
    gameStartPlacement.x = (windowWidth-gameStartPlacement.w)/2;
    gameStartPlacement.y = (windowHeight-gameStartPlacement.h)/2;

    
    SDL_Rect startButton = {gameStartPlacement.x - 30, gameStartPlacement.y - 20, gameStartPlacement.w + 60, gameStartPlacement.h + 40};
    
    
    while(waitingForStart)
    {
        SDL_SetRenderDrawColor(mainWindowRenderer, 0, 0, 0, 255);
        SDL_RenderClear(mainWindowRenderer);

        while (SDL_PollEvent(&event)){
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
                return 0;
            if (event.type == SDL_MOUSEBUTTONDOWN){
                if(event.button.button == SDL_BUTTON_LEFT
                    && event.button.x >= startButton.x
                    && event.button.x <= startButton.x + startButton.w
                    && event.button.y >= startButton.y
                    && event.button.y <= startButton.y + startButton.h)
                    {
                        waitingForStart = false;
                    }
            }
        }

        SDL_SetRenderDrawColor(mainWindowRenderer, 255, 0, 0, 255);
        SDL_RenderFillRect(mainWindowRenderer, &startButton);

        SDL_RenderCopy(mainWindowRenderer, gameStartText, NULL, &gameStartPlacement);

        SDL_RenderPresent(mainWindowRenderer);
    }
    

    while (gameContinues){
        bool uniqueInputUsed = false;
        Uint32 frameTimeStart = SDL_GetTicks();
        
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
                return 0;
            if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.scancode == SDL_SCANCODE_DOWN && !uniqueInputUsed){
                    mainSnake->ChangeDirection(DOWN);
                    uniqueInputUsed = true;
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_UP && !uniqueInputUsed){
                    mainSnake->ChangeDirection(UP);
                    uniqueInputUsed = true; 
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT && !uniqueInputUsed){
                    mainSnake->ChangeDirection(RIGHT);
                    uniqueInputUsed = true;
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT && !uniqueInputUsed){
                    mainSnake->ChangeDirection(LEFT);
                    uniqueInputUsed = true;
                }
            }
        }

        SDL_SetRenderDrawColor(mainWindowRenderer, 0, 0, 0, 255);
        SDL_RenderClear(mainWindowRenderer);

        gameContinues = mainSnake->Move(playground, score, &framerate);
        if (playground->GetFruit() == NULL)
        {
            playground->SpawnFruit();
        }
        
        playgroundGraphics->draw(playground);
        scoreGraphics->draw(score);

        SDL_RenderPresent(mainWindowRenderer);
        
        // si la frame est allée plus vite que le framerate, on attend jusqu'à arriver à framerate, faisant en sorte que
        // chacune de nos frame ait la même durée
        int timeSinceFrameStart = SDL_GetTicks() - frameTimeStart;
        if (timeSinceFrameStart < framerate){
            SDL_Delay(framerate - timeSinceFrameStart);
        }
    }

    //Ecran Game Over
    SDL_SetRenderDrawColor(mainWindowRenderer, 0, 0, 0, 125);
    SDL_RenderClear(mainWindowRenderer);
    SDL_Surface* gameOverSurface = TTF_RenderText_Blended(outsideMainLoopFont, "Game Over !", {255, 0, 0, 255});
    SDL_Texture* gameOverText = SDL_CreateTextureFromSurface(mainWindowRenderer, gameOverSurface);
    SDL_FreeSurface(gameOverSurface);
    SDL_Rect gameOverPlacement;
    SDL_QueryTexture(gameStartText, NULL, NULL, &gameOverPlacement.w, &gameOverPlacement.h);
    gameOverPlacement.x = (windowWidth-gameOverPlacement.w)/2;
    gameOverPlacement.y = (windowHeight-gameOverPlacement.h)/2;
    SDL_RenderCopy(mainWindowRenderer, gameOverText, NULL, &gameOverPlacement);

    SDL_RenderPresent(mainWindowRenderer);
    SDL_Delay(2000);
    
    //supression objets aloués  
    delete mainWindow;
    delete mainSnake;
    delete playground;
    delete playgroundGraphics;
    delete score;
    delete scoreGraphics;
    return 0;
}


