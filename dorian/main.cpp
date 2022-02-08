#include <algorithm>

#include "MainSDLWindow.hpp"
#include "snake.hpp"
#include "score.hpp"
#include "scorerenderer.hpp"
#include "playground.hpp"
#include "playgroundrenderer.hpp"
#include "fruits.hpp"

/*
void waitForStart(SDL_Renderer* mainWindowRenderer, SDL_Event event, TTF_Font*){

}
*/

int main(int argc, char* argv[]){
    int framerate = 100;
    int windowWidth = 800;
    int windowHeight = 800;
    int nbRow = 25;
    int nbCol = 25;
    
    ////////////////////
    // REPLAYING LOOP //
    ////////////////////

    bool replayingGame = true;
    while(replayingGame){
        int framerate = 100;

        MainSDLWindow* mainWindow = new MainSDLWindow();
        bool failedInit = mainWindow->Init("Snake", windowWidth, windowHeight);
        if(failedInit){
            return 1;
        }
        
        SDL_Renderer* mainWindowRenderer = mainWindow->GetRenderer();
        SDL_Event event;

        
        Snake* mainSnake = new Snake(nbRow/4, nbCol/4, RIGHT, std::min(nbRow, nbCol)/6);
        
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

        bool playerControlsInversed = false;
        int timeSinceInversedControls = 0;
        
        TTF_Font* outsideMainLoopFont = TTF_OpenFont("fonts/arial.ttf", 64);
        TTF_Font* outsideMainLoopSmallFont = TTF_OpenFont("fonts/arial.ttf", 32);
        TTF_Font* pixelizedFont = TTF_OpenFont("fonts/pixel.ttf", 64);
        
        /////////////////////
        // WAIT FOR START //
        ////////////////////
        
        
        SDL_Surface* gameStartSurface = TTF_RenderText_Blended(outsideMainLoopFont, "Press any key to start !", {255, 255, 255, 255});
        SDL_Texture* gameStartText = SDL_CreateTextureFromSurface(mainWindowRenderer, gameStartSurface);
        SDL_FreeSurface(gameStartSurface);
        SDL_Rect gameStartPlacement;
        SDL_QueryTexture(gameStartText, NULL, NULL, &gameStartPlacement.w, &gameStartPlacement.h);
        gameStartPlacement.x = (windowWidth-gameStartPlacement.w)/2;
        gameStartPlacement.y = (windowHeight-gameStartPlacement.h)/2;

        
        SDL_Rect startButton = {gameStartPlacement.x - 30, gameStartPlacement.y - 20, gameStartPlacement.w + 60, gameStartPlacement.h + 40};

        SDL_SetRenderDrawColor(mainWindowRenderer, 0, 0, 0, 255);
        SDL_RenderClear(mainWindowRenderer);
        
        playgroundGraphics->draw(playground);
        scoreGraphics->draw(score);

        SDL_SetRenderDrawColor(mainWindowRenderer, 255, 0, 0, 255);
        SDL_RenderFillRect(mainWindowRenderer, &startButton);

        SDL_RenderCopy(mainWindowRenderer, gameStartText, NULL, &gameStartPlacement);

        SDL_RenderPresent(mainWindowRenderer);


        bool waitingForStart = true;
        while(waitingForStart)
        {
            while (SDL_PollEvent(&event)){
                if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
                    return 0;
                if (event.type == SDL_KEYDOWN)
                    waitingForStart = false;
            }
        }


        ///////////////////
        //    MAIN LOOP  //
        ///////////////////

        bool gameContinues = true;
        while (gameContinues){
            bool uniqueInputUsed = false;
            Uint32 frameTimeStart = SDL_GetTicks();

            if(playerControlsInversed){
                timeSinceInversedControls ++;
                if(timeSinceInversedControls == 50){
                    playerControlsInversed = false;
                    timeSinceInversedControls = 0;
                }
            }
            
            while (SDL_PollEvent(&event)){
                if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
                    return 0;
                if (event.type == SDL_KEYDOWN){
                    if (event.key.keysym.scancode == SDL_SCANCODE_DOWN && !uniqueInputUsed){
                        if(playerControlsInversed){
                            mainSnake->ChangeDirection(UP);
                            uniqueInputUsed = true;
                        }
                        else{
                            mainSnake->ChangeDirection(DOWN);
                            uniqueInputUsed = true; 
                        }
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_UP && !uniqueInputUsed){
                        if(playerControlsInversed){
                            mainSnake->ChangeDirection(DOWN);
                            uniqueInputUsed = true;
                        } 
                        else{
                            mainSnake->ChangeDirection(UP);
                            uniqueInputUsed = true;
                        } 
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT && !uniqueInputUsed){
                        if(playerControlsInversed){
                            mainSnake->ChangeDirection(LEFT);
                            uniqueInputUsed = true;
                        }
                        else{
                            mainSnake->ChangeDirection(RIGHT);
                            uniqueInputUsed = true;
                        }
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT && !uniqueInputUsed){
                        if(playerControlsInversed){
                            mainSnake->ChangeDirection(RIGHT);
                            uniqueInputUsed = true;
                        }
                        else{
                            mainSnake->ChangeDirection(LEFT);
                            uniqueInputUsed = true;
                        }
                    }
                }
            }

            SDL_SetRenderDrawColor(mainWindowRenderer, 0, 0, 0, 255);
            SDL_RenderClear(mainWindowRenderer);

            gameContinues = mainSnake->Move(playground, score, &framerate, &playerControlsInversed);
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

        ///////////////////
        //ECRAN GAME OVER//
        ///////////////////

        SDL_SetRenderDrawColor(mainWindowRenderer, 0, 0, 0, 255);
        SDL_RenderClear(mainWindowRenderer);

        // game over text
        SDL_Surface* gameOverSurface = TTF_RenderText_Blended(pixelizedFont, "Game Over !", {255, 0, 0, 255});
        SDL_Texture* gameOverText = SDL_CreateTextureFromSurface(mainWindowRenderer, gameOverSurface);
        SDL_FreeSurface(gameOverSurface);
        SDL_Rect gameOverPlacement;
        SDL_QueryTexture(gameOverText, NULL, NULL, &gameOverPlacement.w, &gameOverPlacement.h);
        gameOverPlacement.x = (windowWidth-gameOverPlacement.w)/2;
        gameOverPlacement.y = windowHeight/4;
        
        //score text
        std::string endScoreString = std::string("Your final score is : " + std::to_string(score->getScore()));
        SDL_Surface* endScoreSurface = TTF_RenderText_Blended(outsideMainLoopSmallFont, endScoreString.c_str(), {255, 255, 255, 255});
        SDL_Texture* endScoreText = SDL_CreateTextureFromSurface(mainWindowRenderer, endScoreSurface);
        SDL_FreeSurface(endScoreSurface);
        SDL_Rect endScorePlacement;
        SDL_QueryTexture(endScoreText, NULL, NULL, &endScorePlacement.w, &endScorePlacement.h);
        endScorePlacement.x = (windowWidth-endScorePlacement.w)/2;
        endScorePlacement.y = gameOverPlacement.y + endScorePlacement.h + windowHeight/10;

        //replay text & buttons
        SDL_Surface* endReplaySurface = TTF_RenderText_Blended(outsideMainLoopSmallFont, "Want to play again ?", {255, 255, 255, 255});
        SDL_Texture* endReplayText = SDL_CreateTextureFromSurface(mainWindowRenderer, endReplaySurface);
        SDL_FreeSurface(endReplaySurface);
        SDL_Rect endReplayPlacement;
        SDL_QueryTexture(endReplayText, NULL, NULL, &endReplayPlacement.w, &endReplayPlacement.h);
        endReplayPlacement.x = (windowWidth-endReplayPlacement.w)/2;
        endReplayPlacement.y = endScorePlacement.y + endReplayPlacement.h + windowHeight/8;

        SDL_SetRenderDrawColor(mainWindowRenderer, 255, 255, 255, 255); //couleur background des boutons
        SDL_Surface* endYesSurface = TTF_RenderText_Blended(pixelizedFont, "Yes", {0, 255, 0, 255});
        SDL_Texture* endYesText = SDL_CreateTextureFromSurface(mainWindowRenderer, endYesSurface);
        SDL_FreeSurface(endYesSurface);
        SDL_Rect endYesPlacement;
        SDL_QueryTexture(endYesText, NULL, NULL, &endYesPlacement.w, &endYesPlacement.h);
        endYesPlacement.x = windowWidth/2 - endYesPlacement.w - windowWidth/10;
        endYesPlacement.y = endReplayPlacement.y + endYesPlacement.h + windowHeight/10;

        SDL_Surface* endNoSurface = TTF_RenderText_Blended(pixelizedFont, "No", {255, 0, 0, 255});
        SDL_Texture* endNoText = SDL_CreateTextureFromSurface(mainWindowRenderer, endNoSurface);
        SDL_FreeSurface(endNoSurface);
        SDL_Rect endNoPlacement;
        SDL_QueryTexture(endNoText, NULL, NULL, &endNoPlacement.w, &endNoPlacement.h);
        endNoPlacement.x = windowWidth/2 + windowWidth/10;
        endNoPlacement.y = endYesPlacement.y;
        
        bool choosingIfReplay = true;
        while(choosingIfReplay){
            SDL_RenderCopy(mainWindowRenderer, gameOverText, NULL, &gameOverPlacement);
            SDL_RenderCopy(mainWindowRenderer, endScoreText, NULL, &endScorePlacement);
            SDL_RenderCopy(mainWindowRenderer, endReplayText, NULL, &endReplayPlacement);
            
            //SDL_RenderFillRect(mainWindowRenderer, &endYesPlacement);
            SDL_RenderCopy(mainWindowRenderer, endYesText, NULL, &endYesPlacement);

            //SDL_RenderFillRect(mainWindowRenderer, &endNoPlacement);
            SDL_RenderCopy(mainWindowRenderer, endNoText, NULL, &endNoPlacement);

            SDL_RenderPresent(mainWindowRenderer);

            while (SDL_PollEvent(&event)){
                if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
                    return 0;
                if (event.type == SDL_MOUSEBUTTONDOWN){
                    if(event.button.button == SDL_BUTTON_LEFT
                        && event.button.x >= endYesPlacement.x
                        && event.button.x <= endYesPlacement.x + endYesPlacement.w
                        && event.button.y >= endYesPlacement.y
                        && event.button.y <= endYesPlacement.y + endYesPlacement.h)
                        {
                            choosingIfReplay = false;
                    }
                    else if(event.button.button == SDL_BUTTON_LEFT
                            && event.button.x >= endNoPlacement.x
                            && event.button.x <= endNoPlacement.x + endNoPlacement.w
                            && event.button.y >= endNoPlacement.y
                            && event.button.y <= endNoPlacement.y + endNoPlacement.h)
                            {
                                replayingGame = false;
                                choosingIfReplay = false;
                    }
                }
            }

            SDL_Delay(framerate);
        }

        //supression objets aloués  
        delete mainWindow;
        delete mainSnake;
        delete playground;
        delete playgroundGraphics;
        delete score;
        delete scoreGraphics;
    }

    return 0;
}


