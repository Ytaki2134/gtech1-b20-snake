#ifndef PLAYGROUND_HPP
#define PLAYGROUND_HPP

class Playground{
    public:
        Playground(int nbCols, int nbRows);
        ~Playground();

        int GetNbCols();
        int GetNbRows();
        void SpawnFruit();

    private:
        int nbCols;
        int nbRows;
        int nbFruitsPresent;
};

#endif