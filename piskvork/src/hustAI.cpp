#include "pisqpipe.h"
#include "engine.h"
#include <windows.h>
#include <array>

const char *infotext="name=\"Random\", author=\"Petr Lastovicka\", version=\"3.2\", country=\"Czech Republic\", www=\"https://plastovicka.github.io\"";

#define MAX_BOARD 20
int board[MAX_BOARD][MAX_BOARD];
static unsigned seed;

// initialize brain
void brain_init() 
{
  if(width>MAX_BOARD || height>MAX_BOARD){
    width = height = 0;
    pipeOut("ERROR Maximal board size is %d", MAX_BOARD);
    return;
  }
  seed=start_time;
  pipeOut("OK");
}

// restart brain
void brain_restart()
{
  int x,y;
  for(x=0; x<width; x++){
    for(y=0; y<height; y++){
      board[x][y]=0;
    }
  }
  pipeOut("OK");
}

int isFree(int x, int y)
{
  return x>=0 && y>=0 && x<width && y<height && board[x][y]==0;
}

void brain_my(int x,int y)
{
  if(isFree(x,y)){
    board[x][y]=1;
  }else{
    pipeOut("ERROR my move [%d,%d]",x,y);
  }
}

void brain_opponents(int x,int y) 
{
  if(isFree(x,y)){
    board[x][y]=2;
  }else{
    pipeOut("ERROR opponents's move [%d,%d]",x,y);
  }
}

// dont need to implement
void brain_block(int x,int y)
{
  if(isFree(x,y)){
    board[x][y]=3;
  }else{
    pipeOut("ERROR winning move [%d,%d]",x,y);
  }
}

// leave it as-is
int brain_takeback(int x,int y)
{
  if(x>=0 && y>=0 && x<width && y<height && board[x][y]!=0){
    board[x][y]=0;
    return 0;
  }
  return 2;
}

void brain_turn() 
{
  std::array<int,2> bestmove = engine(board, 1, info_timeout_turn);
  do_mymove(bestmove[0],bestmove[1]);
}

void brain_end()
{
}

#ifdef DEBUG_EVAL
#include <windows.h>

void brain_eval(int x,int y)
{
  HDC dc;
  HWND wnd;
  RECT rc;
  char c;
  wnd=GetForegroundWindow();
  dc= GetDC(wnd);
  GetClientRect(wnd,&rc);
  c=(char)(board[x][y]+'0');
  TextOut(dc, rc.right-15, 3, &c, 1);
  ReleaseDC(wnd,dc);
}

#endif
