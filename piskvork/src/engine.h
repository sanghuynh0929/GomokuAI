#ifndef engine_hpp
#define engine_hpp
#include <array>
#include <chrono> 

struct Node {
    int score;
    int depth;
    int flag;
    Node() {
        score = 0;
        depth = 0;
        flag = 0;
    }
    Node(int score, int depth, int flag) {
        this->score = score;
        this->depth = depth;
        this->flag = flag;
    }
};
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


std::array<int,2> engine(int GameBoard[20][20], int player, int time);
#endif