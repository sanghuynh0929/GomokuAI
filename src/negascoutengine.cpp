#include "negascoutengine.hpp"

// negascout_depth_limited.cpp : Defines the entry point for the application.

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <map>
#include <time.h>
#include <chrono>
#include <random>

using namespace std;

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

map<int, int> ST;
map<int, Node> TT;
int maxdepth = 0;
Move tempbest;

typedef chrono::time_point<chrono::high_resolution_clock> TimePoint;
typedef array<int,4> bounds;
const int INF = (int) 1e9;
const int FORCING = - INF - 10;
const int LIVE[] = {1,10,100,1000,10000,100000};
const int DEAD[] = {0, 1, 10, 100, 1000, 100000};

int evaluate_block(int blocks, int pieces) {
    if (blocks == 0) {
        if(pieces <= 5)
            return LIVE[pieces];
        else
            return LIVE[5];
    }
    else if (blocks == 1) {
        if(pieces <= 5)
            return DEAD[pieces];
        else
            return DEAD[5];
    }
    else {
        if (pieces >= 5)
            return LIVE[5];
        else
            return 0;
    }
}

int evaluate_board(int Board[20][20], int pieceType, bounds const& restrictions) {
    int score = 0;
    int mnr = restrictions[0];
    int mnc = restrictions[1];
    int mxr = restrictions[2];
    int mxc = restrictions[3];
    for (int row = mnr; row < mxr + 1; row++) {
        for (int col = mnc; col < mxc + 1; col++) {
            if (Board[row][col] == pieceType) {
                int block = 0;
                int piece = 1;
                // left
                if (col == 0 or Board[row][col - 1] != 0) {
                    block++;
                }
                // pieceNum
                col++;
                for (; col < 20 and Board[row][col] == pieceType; col++) {
                    piece++;
                }
                // right
                if (col == 20 or Board[row][col] != 0) {
                    block++;
                }
                score += evaluate_block(block, piece);
            }
        }
    }

    for (int col = mnc; col < mxc + 1; col++) {
        for (int row = mnr; row < mxr + 1; row++) {
            if (Board[row][col] == pieceType) {
                int block = 0;
                int piece = 1;
                // left
                if (row == 0 or Board[row - 1][col] != 0) {
                    block++;
                }
                // pieceNum
                row++;
                for (; row < 20 and Board[row][col] == pieceType; row++) {
                    piece++;
                }
                // right
                if (row == 20 or Board[row][col] != 0) {
                    block++;
                }
                score += evaluate_block(block, piece);
            }
        }
    }

    for (int n = mnr; n < (mxc - mnc + mxr); n++) {
        int r = n;
        int c = mnc;
        while (r >= mnr and c <= mxc) {
            if (r <= mxr) {
                if (Board[r][c] == pieceType) {
                    int block = 0;
                    int piece = 1;
                    // left
                    if (c == 0 or r == 20 - 1 or Board[r + 1][c - 1] != 0) {
                        block++;
                    }
                    // pieceNum
                    r--;
                    c++;
                    for (; r >= 0 and Board[r][c] == pieceType; r--) {
                        piece++;
                        c++;
                    }
                    // right
                    if (r < 0 or c == 20 or Board[r][c] != 0) {
                        block++;
                    }
                    score += evaluate_block(block, piece);
                }
            }
            r--;
            c++;
        }
    }

    for (int n = mnr - (mxc - mnc); n <= mxr; n++) {
        int r = n;
        int c = mnc;
        while (r <= mxr and c <= mxc) {
            if (r >= mnr and r <= mxr) {
                if (Board[r][c] == pieceType) {
                    int block = 0;
                    int piece = 1;
                    // left
                    if (c == 0 or r == 0 or Board[r - 1][c - 1] != 0) {
                        block++;
                    }
                    // pieceNum
                    r++;
                    c++;
                    for (; r < 20 and Board[r][c] == pieceType; r++) {
                        piece++;
                        c++;
                    }
                    // right
                    if (r == 20 or c == 20 or Board[r][c] != 0) {
                        block++;
                    }
                    score += evaluate_block(block, piece);
                }
            }
            r++;
            c++;
        }

    }
    return score;
}

vector<array<int, 9>> get_directions(int Board[20][20], int x, int y) {
    array<int, 9> a, b, c, d;
    int a_i = 0, b_i = 0, c_i = 0, d_i = 0;
    
    for(int i = x - 4; i <= x + 4; i++){
        if(i < 0 or i >= 20)
            a[a_i++] = 2;
        else
            a[a_i++] = Board[i][y];
    }
    
    for(int j = y - 4; j <= y + 4; j++){
        if(j < 0 or j >= 20)
            b[b_i++] = 2;
        else
            b[b_i++] = Board[x][j];
    }
    for(int i = x-4, j = y-4; i <= x + 4 and j <= y + 4; i++, j++){
        if(i < 0 or i >= 20 or j < 0 or j >= 20)
            c[c_i++] = 2;
        else
            c[c_i++] = Board[i][j];
    }
    for(int i = x-4, j = y+4; i <= x + 4 and j >= y - 4; i++, j--){
        if(i < 0 or i >= 4 or j < 0 or j >= 4)
            d[d_i++] = 2;
        else
            d[d_i++] = Board[i][j];
    }
    
    vector<array<int, 9>> Directions = {a,b,c,d};
    return Directions;
}

bool check_directions(const array<int, 9> & arr) {
    int size = 9;
    for (int i = 0; i < size - 4; i++) {
        if (arr[i] != 0) {
            if (arr[i] == 2 or arr[i + 1] == 2 or arr[i + 2] == 2 or arr[i + 3] == 2 or arr[i + 4] == 2) {
                return false;
            }
            if (arr[i] == arr[i + 1] and arr[i] == arr[i + 2] and arr[i] == arr[i + 3] and arr[i] == arr[i + 4]) {
                return true;
            }
        }
    }
    return false;
}

bool check_win(int Board[20][20], int x, int y) {
    vector<array<int, 9>> Directions = get_directions(Board, x, y);
    for (auto &dir : Directions) {
        if (check_directions(dir)) {
            return true;
        }
    }
    return false;
}


bool local_cell(int Board[20][20], int r, int c) {
    for (int i = r - 2; i <= r + 2; i++) {
        if (i < 0 or i >= 20) continue;
        for (int j = c - 2; j <= c + 2; j++) {
            if (j < 0 or j >= 20) continue;
            if (Board[i][j] != 0)
                return true;
        }
    }
    return false;
}

bounds get_bounds(int Board[20][20]) {
    int mnr = 20;
    int mnc = 20;
    int mxr = -1;
    int mxc = -1;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (Board[i][j] != 0) {
                mnr = min(mnr, i);
                mnc = min(mnc, j);
                mxr = max(mxr, i);
                mxc = max(mxc, j);
            }
        }
    }
    mnr = max(mnr, 2);
    mnc = max(mnc, 2);
    mxr = min(mxr, 17);
    mxc = min(mxc, 17);
    return bounds{mnr, mnc, mxr, mxc};
}

bounds update_restrictions(bounds const& restrictions, int i, int j) {
    int mnr = restrictions[0];
    int mnc = restrictions[1];
    int mxr = restrictions[2];
    int mxc = restrictions[3];
    mnr = min(mnr, i);
    mnc = min(mnc, j);
    mxr = max(mxr, i);
    mxc = max(mxc, j);
    mnr = max(mnr, 2);
    mnc = max(mnc, 2);
    mxr = min(mxr, 17);
    mxc = min(mxc, 17);
    bounds updated = {mnr, mnc, mxr, mxc};
    return updated;
}


const int YBLOCK[] = {0,40,800,20000,800000};
const int EBLOCK[] = {0,20,1000,100000,800000};

int evaluate_config(int you, int enemy)
{
    if(you != 0 and enemy != 0)
        return 0; // block with 2 colors yield no points
    if(you == 0 and enemy == 0)
        return 10;
    if(you != 0 and you < 5)
        return YBLOCK[you];
    if(enemy != 0 and enemy < 5)
        return EBLOCK[enemy];
    return 0;
}

int evaluate_state(int Board[20][20], int player, int hash, bounds const& restrictions) {
    int black_score = evaluate_board(Board, -1, restrictions);
    int white_score = evaluate_board(Board, 1, restrictions);
    int score = 0;
    if (player == -1) {
        score = (black_score - white_score);
    }
    else {
        score = (white_score - black_score);
    }
    ST[hash] = score;
    return score;
}


int evaluate_direction(const array<int, 9> & dir, int player) {
    // evaluate the following blocks
    // dir: -4 -3 -2 -1 0 1 2 3 4
    // blocks:
    // (-4, -3, -2, -1, 0)
    // (-3, -2, -1,  0, 1)
    // (-2, -1,  0,  1, 2)
    // (-1,  0,  1,  2, 3)
    // ( 0,  1,  2,  3, 4)
    int score = 0;
    int i = 0;
    while(dir[i] == 2) i++;
    for (; i + 4 < 9; i++) {
        int you = 0;
        int enemy = 0;
        if (dir[i] == 2) {
            return score;
        }
        for (int j = 0; j <= 4; j++) {
            if (dir[i + j] == 2) {
                return score;
            }
            else if (dir[i + j] == player) {
                you++;
            }
            else if (dir[i + j] == -player) {
                enemy++;
            }
        }
        score += evaluate_config(you, enemy);
        if (score >= 800000) {
            return FORCING;
        }
    }
    return score;
}


int evaluate_move(int Board[20][20], int x, int y, int player) {
    int score = 0;
    vector<array<int, 9>> Directions = get_directions(Board, x, y);
    int temp_score;
    for (auto &dir : Directions) {
        temp_score = evaluate_direction(dir, player);
        if (temp_score == FORCING) {
            return FORCING;
        }
        else {
            score += temp_score;
        }
    }
    return score;
}

bool move_sorter(Move const& move1, Move const& move2) {
    return move1.score > move2.score;
}

vector<Move> generate_moves(bounds const& restrictions, int Board[20][20], int player)
{
    vector<Move> candidate_moves;
    int mnr = restrictions[0];
    int mnc = restrictions[1];
    int mxr = restrictions[2];
    int mxc = restrictions[3];
    for (int i = mnr - 2; i <= mxr + 2; i++) {
        for (int j = mnc - 2; j <= mxc + 2; j++) {
            if (Board[i][j] == 0 and local_cell(Board, i, j)) {
                Move move;
                move.i = i;
                move.j = j;
                Board[i][j] = -player;
                if(check_win(Board,i,j)){
                    vector<Move> blocking_move(1,move);
                    Board[i][j] = 0;
                    return blocking_move;
                }
                Board[i][j] = player;
                if(check_win(Board,i,j)){
                    vector<Move> winning_move(1,move);
                    Board[i][j] = 0;
                    return winning_move;
                }
                Board[i][j] = 0;
                move.score = evaluate_move(Board, i, j, player);
                if (move.score == FORCING) {
                    vector<Move> winning_move(1,move);
                    return winning_move;
                }
                candidate_moves.push_back(move);
            }
        }
    }

    sort(candidate_moves.begin(), candidate_moves.end(), move_sorter);
    //  return availSpots_score.slice(0,20) // testing Beam Search
    return candidate_moves;
}
int Table[20][20][2];

mt19937 mt_rand((unsigned) time(nullptr));

void Table_init()
{
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            Table[i][j][0] = mt_rand(); //1
            Table[i][j][1] = mt_rand(); //2
        }
    }
}

int get_hash(int board[20][20])
{
    int h = 0;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            int Board_value = board[i][j];
            if (Board_value != 0) {
                if (Board_value == -1)
                    h ^= Table[i][j][0];
                else
                    h ^= Table[i][j][1];
            }
        }
    }
    return h;
}

int update_hash(int hash, int player, int i, int j)
{
    if (player == -1)
        hash ^= Table[i][j][0];
    else
        hash ^= Table[i][j][1];
    return hash;
}

TimePoint start;
int tlimit = 0;
bool timeout(){
    TimePoint t = chrono::high_resolution_clock::now();
    auto delta = chrono::duration_cast<chrono::milliseconds>(t - start);
    if(delta.count() >= tlimit)
        return true;
    else
        return false;
}

int negascout(int Board[20][20], int player, int depth, int alpha, int beta, int hash, const bounds& restrictions, int last_i, int last_j)
{
    if(timeout()) {
        return 1;
    }
    int a = alpha;
    if (TT.count(hash) and (TT[hash].depth >= depth)) {
        // if position is in transposition table
        int score = TT[hash].score;
        switch(TT[hash].flag) {
            case 1:
                // position is an upper bound
                beta = min(beta, score);
                break;
            case -1:
                // position is a lower bound
                alpha = max(alpha, score);
                break;
            default: // case 0:
                return score;
        }
        if (alpha >= beta)
            return score;
    }
    if (last_i >= 0 and last_j >= 0 and check_win(Board, last_i, last_j)) {
        return -2000000 + (maxdepth - depth); // prioritize quicker win variation
    }
    if (depth == 0) {
        if (ST.count(hash))
            return ST[hash];
        else
            return evaluate_state(Board, player, hash, restrictions);
    }
    vector<Move> candidate_moves = generate_moves(restrictions, Board, player);
    int num_moves = (int) candidate_moves.size();
    if (num_moves == 0)
        return 0;
    
    int b = beta;
    int score = -INF;
    for (int moveid = 0; moveid < num_moves; moveid++) {
        int i = candidate_moves[moveid].i;
        int j = candidate_moves[moveid].j;
        // Make move
        Board[i][j] = player;
        
        int new_hash = update_hash(hash, player, i, j);
        bounds new_restrictions = update_restrictions(restrictions, i, j);
        int t = -negascout(Board, -player, depth-1, -b, -alpha, new_hash, new_restrictions, i, j);
        if (alpha < t and t < beta and moveid > 0) {
            t = -negascout(Board, -player, depth-1, -beta, -t, new_hash, new_restrictions, i, j);
        }
        if (t > score) {
            score = t;
            if (depth == maxdepth) {
                tempbest.i = i;
                tempbest.j = j;
                tempbest.score = t;
            }
        }
        // Reset board
        Board[i][j] = 0;
        alpha = max(alpha,t);
        if (alpha >= beta)
            break;
        
        // Setup null window
        b = alpha + 1;
    }
    Node cache(score, depth, 0);
    if (score <= a)
        cache.flag = 1; // Node.score an upper bound of position's value
    else if (score >= b)
        cache.flag = -1; // Node.score a lower bound of position's value
    TT[hash] = cache;
    return score;
}

Move engine(int Board[20][20], int player, int time)
{
    Table_init();
    tlimit = time * 1000;
    ::start = chrono::high_resolution_clock::now();
    // Iterative Framework
    int depth = 2;
    Move bestmove(-1,-1,0);
    while(!timeout()){
        maxdepth = depth;
        negascout(Board, player, maxdepth, -INF, INF, get_hash(Board), get_bounds(Board), -1, -1);
        if(timeout()){
            break;
        }
        bestmove = tempbest;
        if(bestmove.score > 1999900){
            break;
        }
        depth += 2;
    }
    TT.clear();
    ST.clear();
    return bestmove;
}
