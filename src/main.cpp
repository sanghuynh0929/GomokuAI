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
