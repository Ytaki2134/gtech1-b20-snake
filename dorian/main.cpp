#include "MainSDLWindow.hpp"
#include "snake.hpp"
#include "score.hpp"
#include "scorerenderer.hpp"
#include "playground.hpp"
#include "playgroundrenderer.hpp"
#include "fruits.hpp"

int main(int argc, char* argv[]){
    Uint32 framerate = 40;
    int windowSize = 800;
    int tile_size = 20; 
    bool gameContinues = true;
    
    MainSDLWindow* mainWindow = new MainSDLWindow();
    bool failedInit = mainWindow->Init("Snake", 800, 800);
    if(failedInit){
        return 1;
    }
    
    SDL_Renderer* mainWindowRenderer = mainWindow->GetRenderer();
    SDL_Event event;

    
    Snake* mainSnake = new Snake(25, 25, RIGHT, 5);

    Playground* playground = new Playground(mainWindow->GetPlaygroundZone().w/tile_size, mainWindow->GetPlaygroundZone().h/tile_size, mainSnake);
    PlaygroundRenderer* playgroundGraphics = new PlaygroundRenderer();
    failedInit = playgroundGraphics->Init(mainWindowRenderer, mainWindow->GetPlaygroundZone(), tile_size);

    if(failedInit){
        return 1;
    }

    Score* score = new Score();
    ScoreRenderer* scoreGraphics = new ScoreRenderer();
    failedInit = scoreGraphics->Init(mainWindowRenderer, mainWindow->GetScoreZone());
    if(failedInit){
        return 1;
    }

    while (gameContinues){
        Uint32 frameTimeStart = SDL_GetTicks();

        SDL_SetRenderDrawColor(mainWindowRenderer, 0, 0, 0, 255);
        SDL_RenderClear(mainWindowRenderer);

        gameContinues = mainSnake->Move(playground);
        if (playground->GetFruit() == NULL)
        {
            playground->SpawnFruit();
        }
    

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
        
        playgroundGraphics->drawGrid();
        playgroundGraphics->drawFruit(playground->GetFruit());
        playgroundGraphics->drawSnake(playground->GetSnake());
        scoreGraphics->draw(score);

        SDL_RenderPresent(mainWindowRenderer);
        

        Uint32 timeSinceFrameStart = SDL_GetTicks() - frameTimeStart;
        if (timeSinceFrameStart < framerate){
            SDL_Delay(framerate - timeSinceFrameStart);
        }
    }
    
    delete mainWindow;
    return 0;
}