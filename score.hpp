#ifndef SCORE_HPP
#define SCORE_HPP

class Score{
    public:
        Score();
        ~Score();
        
        void update_numb_eaten(int value);
        void update_score(double value);

        int getScore();
        int getNumberEaten();
    
    private:
        int number_eaten;
        double score;
};

#endif