#include <iostream>
#include <array>
#include "negascoutengine.hpp"

using namespace std;

// Freestyle
bool winning(int Board[20][20], int x, int y) {
    if(x < 0 or y < 0)
        return false;
    
    // Check row:
    // x-4 x-3 x-2 x-1 x x+1 x+2 x+3 x+4
    for(int i = x - 4; i <= x; i++) {
        if(i < 0 or i + 4 >= 20) continue;
        if(Board[x][y] == Board[i][y] and Board[x][y] == Board[i+1][y] and Board[x][y] == Board[i+2][y] and Board[x][y] == Board[i+3][y] and Board[x][y] == Board[i+4][y]){
            return true;
        }
    }
    // Check column:
    // y-4 y-3 y-2 y-1 y y+1 y+2 y+3 y+4
    for(int i = y - 4; i <= y; i++) {
        if(i < 0 or i + 4 >= 20) continue;
        if(Board[x][y] == Board[x][i] and Board[x][y] == Board[x][i+1] and Board[x][y] == Board[x][i+2] and Board[x][y] == Board[x][i+3] and Board[x][y] == Board[x][i+4]){
            return true;
        }
    }
    // Check diagonal
    // x-4 x-3 x-2 x-1 x x+1 x+2 x+3 x+4
    // y-4 y-3 y-2 y-1 y y+1 y+2 y+3 y+4
    for(int i = x-4, j = y-4; i <= x and j <= y; i++, j++) {
        if(i < 0 or j < 0 or i + 4 >= 20 or j + 4 >= 20) continue;
        if(Board[x][y] == Board[i][j] and Board[x][y] == Board[i+1][j+1] and Board[x][y] == Board[i+2][j+2] and Board[x][y] == Board[i+3][j+3] and Board[x][y] == Board[i+4][j+4]){ return true;
        }
    }
    // Check diagonal
    // x+4 x+3 x+2 x+1 x x-1 x-2 x-3 x-4
    // y-4 y-3 y-2 y-1 y y+1 y+2 y+3 y+4
    for(int i = x+4, j = y-4; i >= x and j <= y; i--, j++) {
        if(i - 4 < 0 or j < 0 or i >= 20 or j + 4 >= 20) continue;
        if(Board[x][y] == Board[i][j] and Board[x][y] == Board[i-1][j+1] and Board[x][y] == Board[i-2][j+2] and Board[x][y] == Board[i-3][j+3] and Board[x][y] == Board[i-4][j+4]){
            return true;
        }
    }
    return false;
}

int Search(int Board[20][20], int player, int time, int xprev, int yprev) {
    int xcount = 0, ocount = 0;
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 20; j++) {
            if(Board[i][j] == 1) xcount++;
            if(Board[i][j] == -1) ocount++;
        }
    }
    if(winning(Board,xprev,yprev)) {
        return -player;
        // Human wins
    }
    if(xcount + ocount == 400) return 2;
    if(xcount + ocount == 0) {
        Board[10][10] = player;
        return 0;
    }
    Move bestmove = engine(Board, player, time);
    Board[bestmove.i][bestmove.j] = player;
    cout << "AI played: " << bestmove.i << " " << bestmove.j << endl;
    if(winning(Board,bestmove.i,bestmove.j)) {
        return player;
        // AI wins
    }
    return 0;
}

void PrintBoard(int Board[20][20]){
    system("clear");
    cout << "                       1 1 1 1 1 1 1 1 1 1" << endl;
    cout << "   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9" << endl;
    for(int i = 0; i < 20; i++) {
        if(i < 10)
            cout << " " << i << " ";
        else
            cout << i << " ";
        for(int j = 0; j < 20; j++) {
            if(Board[i][j] == 0)
                cout << "_ ";
            else if (Board[i][j] == 1)
                cout << "X ";
            else
                cout << "O ";
        }
        cout << endl;
    }
    return;
}

void Game(int player, int time) {
    int Board[20][20] = {};
    for(int i = 0; i < 20; i++) {
        for(int j = 0; j < 20; j++) {
            Board[i][j] = 0;
        }
    }
    cout << "Press any key to start:" << endl;
    int x = -1, y = -1;
    while(true) {
        if(player == -1) {
            int end = Search(Board, -player, time, x, y);
            if(end == 1){
                PrintBoard(Board);
                cout << "X has won" << endl;
                return;
            }
            else if(end == -1){
                PrintBoard(Board);
                cout << "O has won" << endl;
                return;
            }
            else if(end == 2){
                PrintBoard(Board);
                cout << "Game ended in a draw" << endl;
                return;
            }
        }
        PrintBoard(Board);
        cout << "Enter the coordinate of move (Row Column): ";
        cin >> x >> y;
        if(Board[x][y] != 0) {
            continue;
        }
        Board[x][y] = player;
        if(player == 1) {
            int end = Search(Board, -player, time, x, y);
            if(end == 1){
                PrintBoard(Board);
                cout << "X has won" << endl;
                return;
            }
            else if(end == -1){
                PrintBoard(Board);
                cout << "O has won" << endl;
                return;
            }
            else if(end == 2){
                PrintBoard(Board);
                cout << "Game ended in a draw" << endl;
                return;
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    int player = 0;
    int time = 5;
    while(true) {
        cout << "Choose your color (X/O): ";
        string c; cin >> c;
        if(c == "X" or c == "x"){
            player = 1;
            break;
        }
        if(c == "O" or c == "o"){
            player = -1;
            break;
        }
    }
    cout << "Enter time for AI ([1,120]) (s): ";
    string t; cin >> t;
    int tmp = stoi(t);
    if(tmp > 120 or tmp < 1) {
        cout << "Time is set to default: 5s per move" << endl;
    }
    else {
        cout << "Time is set to " << tmp << "s per move" << endl;
        time = tmp;
    }
    Game(player, time);
    return 0;
}
