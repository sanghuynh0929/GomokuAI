#ifndef negascoutengine_hpp
#define negascoutengine_hpp

struct Move {
    int i;
    int j;
    int score;
    Move(){
        return;
    }
    Move(int i, int j, int score){
        this->i = i;
        this->j = j;
        this->score = score;
    }
};

Move engine(int Board[20][20], int player, int time);
#endif
