#include <time.h>
#include "playground.hpp"

Playground::Playground(int nbCols, int nbRows, Snake* snake){
    this->nbCols = nbCols;
    this->nbRows = nbRows;
    
    this->SpawnFruit(snake);
}

Playground::~Playground(){

}

int Playground::GetNbCols(){
    return nbCols;
}

int Playground::GetNbRows(){
    return nbRows;
}

Fruit* Playground::GetFruit(){
    return presentFruit;
}

void Playground::SpawnFruit(Snake* snake){
    srand(time(NULL));
    int randomCol = rand()%nbCols;
    int randomRow = rand()%nbRows;

    FruitEffect randomEffect = static_cast<FruitEffect>(rand() % COUNT);

    bool correct_position = true;

    //vérifier que les coordonnées créées aléatoirement sont correcte (pas sur le serpent) et si ce n'est pas le cas
    //en générer de nouvelles
    do
    {
        correct_position = true;
        Segment* actualSegment = snake->GetHead();

        //parcourir tout le serpent et comparer les coordonnées de chaque segment avec les coordonnées de la pomme
        while (actualSegment != NULL)
        {
            if (actualSegment->GetCol() == randomCol && actualSegment->GetRow() == randomRow)
            {
                correct_position = false;
                break;
            }
            
            actualSegment = actualSegment->GetNext();
        }

        //générer nouvelles coords si elles sont incorrectes
        if(!correct_position){
            randomCol = rand()%nbCols;
            randomRow = rand()%nbRows;
        }
        
    } while (!correct_position);
    
    //si on est sorti de la boucle while alors les coordonnées sont correctes donc on peut créer la pomme
    presentFruit = new Fruit(randomRow, randomCol, randomEffect);
}

