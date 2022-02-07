
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
    
    //detruire toutes les textures des différents tableaux de textures du serpent
    for(int i=0; i<sizeof(snakeHeadTextures) / sizeof(snakeHeadTextures[0]);i++){
        SDL_DestroyTexture(snakeHeadTextures[i]);
    }

    for(int i=0; i<sizeof(snakeTailTextures) / sizeof(snakeTailTextures[0]);i++){
        SDL_DestroyTexture(snakeTailTextures[i]);
    }

    for(int i=0; i<sizeof(snakeBodyTextures) / sizeof(snakeBodyTextures[0]);i++){
        SDL_DestroyTexture(snakeBodyTextures[i]);
    }
}

//dessine le cadrillage, le fruit et le snake.
void PlaygroundRenderer::draw(Playground* playground){
    drawBackground();
    drawFruit(playground->GetFruit());
    drawSnake(playground->GetSnake());
}

//dessine le cadrillage
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
    SDL_RenderSetViewport(renderer, &drawZone);
    Segment* actual_segment = snakeToDraw->GetHead();

    drawSnakeHead(actual_segment);
    actual_segment = actual_segment->GetNext();
    while(actual_segment->GetNext() != NULL){
        drawSnakeBodySegment(actual_segment);
        actual_segment = actual_segment->GetNext();
    }
    drawSnakeTail(actual_segment);
    
    SDL_RenderSetViewport(renderer, NULL);
}

void PlaygroundRenderer::drawSnakeHead(Segment* segment){
    SDL_Rect rectToDraw = {segment->GetCol()*tileSize, segment->GetRow()*tileSize, tileSize, tileSize};
    SDL_RenderCopy(renderer, snakeHeadTextures[segment->GetDirection()], NULL, &rectToDraw);
}

void PlaygroundRenderer::drawSnakeBodySegment(Segment* segment){
    SDL_Rect rectToDraw = {segment->GetCol()*tileSize, segment->GetRow()*tileSize, tileSize, tileSize};
    SDL_RenderCopy(renderer, snakeBodyTextures[segment->GetDirection()], NULL, &rectToDraw);
}

void PlaygroundRenderer::drawSnakeTail(Segment* segment){
    SDL_Rect rectToDraw = {segment->GetCol()*tileSize, segment->GetRow()*tileSize, tileSize, tileSize};
    SDL_RenderCopy(renderer, snakeTailTextures[segment->GetDirection()], NULL, &rectToDraw);
}

//initialise toutes les valeurs requises au fonctionnement du PlaygroundRenderer
int PlaygroundRenderer::Init(SDL_Renderer* renderer, SDL_Rect drawZone, Playground* playground){
    this->renderer = renderer;
    this->drawZone = drawZone;
    this->playground = playground;

    //calcule la tileSize en fonction du nombre de col/rows et de la width/height de la drawzone du playground
    int tileWidth = floor(drawZone.w/playground->GetNbCols());
    int tileHeight = floor(drawZone.h/playground->GetNbRows());
    this->tileSize = std::min(tileHeight,tileWidth);
    
    //change la drawzone pour qu'elle soit de la bonne taille et la place au milieu de l'écran
    this->drawZone.w = tileSize * playground->GetNbCols();
    this->drawZone.h = tileSize * playground->GetNbRows();
    this->drawZone.y += (drawZone.h - this->drawZone.h) / 2;
    this->drawZone.x += (drawZone.w - this->drawZone.w) / 2;
    

    bool initBackgroundFailed = InitBackground();
    if (initBackgroundFailed){
        return EXIT_FAILURE;
    }

    bool initSnakeTexturesFailed = InitSnakeTextures();
    if (initSnakeTexturesFailed){
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

//load une image à partir d'un nom de fichier et renvoie la texture créée à partir de cette dernière
SDL_Texture* PlaygroundRenderer::LoadTexture(const std::string* filename){
    SDL_Surface* imageSurf = SDL_LoadBMP(filename->c_str());
    if(imageSurf == NULL){
        printf("Problem with loading image: %s\n", SDL_GetError());
    }
	SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurf);
    if(imageTexture == NULL){
        printf("Problem with creating texture: %s\n", SDL_GetError());
    }
	SDL_FreeSurface(imageSurf);
    return imageTexture;
}

//dessine le cadrillage sur une texture et enregistre cette derrière dans l'attribut bgTexture
int PlaygroundRenderer::InitBackground(){
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

//initialise les attributs snakeHeadTextures, snakeHeadTextures et snakeTailTextures qui sont utilisés pour dessiner le serpent
int PlaygroundRenderer::InitSnakeTextures(){
    //listes des noms  des images pour créer les textures du serpent
    const std::string headTextureNames[] = {"head_down.bmp", "head_up.bmp", "head_right.bmp", "head_left.bmp"};
    const std::string bodyTextureNames[] = {"body_down.bmp", "body_up.bmp", "body_right.bmp", "body_left.bmp",
                                            "body_downright.bmp", "body_downleft.bmp", "body_upright.bmp", "body_upleft.bmp",
                                            "body_rightdown.bmp", "body_rightup.bmp", "body_leftdown.bmp", "body_leftup.bmp"};
    const std::string tailTextureNames[] = {"tail_down.bmp", "tail_up.bmp", "tail_right.bmp", "tail_left.bmp",
                                            "tail_downright.bmp", "tail_downleft.bmp", "tail_upright.bmp", "tail_upleft.bmp",
                                            "tail_rightdown.bmp", "tail_rightup.bmp", "tail_leftdown.bmp", "tail_leftup.bmp"};
    

    //à chacun des éléments des tableaux de textures du PlaygroundRenderer, on associe une texture créée à 
    //partir d'un des noms d'images des tableaux au dessus
    for(int i=0; i<sizeof(headTextureNames) / sizeof(headTextureNames[0]);i++){
        std::string fullPath = std::string("images/") + headTextureNames[i];
        SDL_Texture* loadedTexture = LoadTexture(&fullPath);
        if(loadedTexture == NULL){
            return EXIT_FAILURE;
        }
        snakeHeadTextures[i] = loadedTexture;
    }

    for(int i=0; i<sizeof(bodyTextureNames) / sizeof(bodyTextureNames[0]);i++){
        std::string fullPath = std::string("images/") + bodyTextureNames[i];
        SDL_Texture* loadedTexture = LoadTexture(&fullPath);
        if(loadedTexture == NULL){
            return EXIT_FAILURE;
        }
        snakeBodyTextures[i] = loadedTexture;
    }

    for(int i=0; i<sizeof(tailTextureNames) / sizeof(tailTextureNames[0]);i++){
        std::string fullPath = std::string("images/") + tailTextureNames[i];
        SDL_Texture* loadedTexture = LoadTexture(&fullPath);
        if(loadedTexture == NULL){
            return EXIT_FAILURE;
        }
        snakeTailTextures[i] = loadedTexture;
    }

    return EXIT_SUCCESS;
}