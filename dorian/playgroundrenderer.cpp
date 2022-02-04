
#include "playgroundrenderer.hpp"
#include "playground.hpp"
#include <cmath>
#include <algorithm>

PlaygroundRenderer::PlaygroundRenderer(){
    renderer = NULL;
    bgTexture = NULL;
    playground = NULL;
    drawZone = {0, 0, 0, 0};
    tileSize = 0;
}

PlaygroundRenderer::~PlaygroundRenderer(){
    
}

void PlaygroundRenderer::draw(Playground* playground){
    drawBackground();
    drawFruit(playground->GetFruit());
    drawSnake(playground->GetSnake());
}

void PlaygroundRenderer::drawBackground(){
    SDL_RenderCopy(renderer, bgTexture, NULL, &drawZone);
}

void PlaygroundRenderer::drawFruit(Fruit* fruitToDraw){

    //couleurs correspondant dans l'ordre des différents effets dans l'enum FruitEffect
    SDL_Color effectColors [3] = {{255, 0, 0, 255}, {189, 183, 107, 255}, {255, 255, 0, 255}}; //red, kaki, yellow
    SDL_RenderSetViewport(renderer, &drawZone);

    //on utilise le fait que les valeurs des effets FruitEffect s'incrémentent à partir de 0 comme les index d'un tableau 
    SDL_SetRenderDrawColor(renderer, effectColors[fruitToDraw->GetEffect()].r, effectColors[fruitToDraw->GetEffect()].g, effectColors[fruitToDraw->GetEffect()].b, effectColors[fruitToDraw->GetEffect()].a);

    SDL_Rect fruitRect = {fruitToDraw->GetCol() * tileSize, fruitToDraw->GetRow() * tileSize, tileSize, tileSize};
    SDL_RenderFillRect(renderer, &fruitRect);
}

void PlaygroundRenderer::drawSnake(Snake* snakeToDraw){
    Segment* actual_segment = snakeToDraw->GetHead();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderSetViewport(renderer, &drawZone);

    while(actual_segment != nullptr)
    {
        SDL_Rect rectToDraw = {actual_segment->GetCol()*tileSize, actual_segment->GetRow()*tileSize, tileSize, tileSize};
        SDL_RenderFillRect(renderer, &rectToDraw);
        actual_segment = actual_segment->GetNext();
    }
    
    SDL_RenderSetViewport(renderer, NULL);
}

int PlaygroundRenderer::Init(SDL_Renderer* renderer, SDL_Rect drawZone, Playground* playground){
    this->renderer = renderer;
    this->drawZone = drawZone;
    this->playground = playground;

    int tileWidth = floor(drawZone.w/playground->GetNbCols());
    int tileHeight = floor(drawZone.h/playground->GetNbRows());
    this->tileSize = std::min(tileHeight,tileWidth);
    
    this->drawZone.w = tileSize * playground->GetNbCols();
    this->drawZone.h = tileSize * playground->GetNbRows();
    this->drawZone.y += (drawZone.h - this->drawZone.h) / 2;
    this->drawZone.x += (drawZone.w - this->drawZone.w) / 2;
    

    bool initBackgroundFailed = InitBackground();
    if (initBackgroundFailed){
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

}

int PlaygroundRenderer::InitBackground(){
/*	SDL_Surface* bmpSurf = SDL_LoadBMP("test.bmp");
	bgTexture = SDL_CreateTextureFromSurface(renderer, bmpSurf);
    if(bgTexture == NULL){
        printf("Problem with creating texture : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
	SDL_FreeSurface(bmpSurf);
    return EXIT_SUCCESS;*/
    bgTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, drawZone.w, drawZone.h);
    
    if(bgTexture == NULL){
        printf("Problem with creating texture : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

	SDL_SetRenderTarget(renderer, bgTexture);


    int row = 0;
    int col = 0;

    SDL_SetRenderDrawColor(renderer, 21, 71, 52, 255);
    
    while(row < playground->GetNbRows()){
        SDL_Rect actual_tile = {col*tileSize, row*tileSize, tileSize, tileSize};
        SDL_RenderDrawRect(renderer, &actual_tile);

        col ++;

        if(col >= playground->GetNbCols()){
            row ++;
            col = 0;
        }
    }
	
	SDL_SetRenderTarget(renderer, NULL);
    return EXIT_SUCCESS;
}