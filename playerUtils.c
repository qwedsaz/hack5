#include <stdlib.h>
#include <stdio.h>

#include "gameUtils.h"
#include "playerUtils.h"

void userMove(Move **board, Move player) {

  int input = 0;
  while(input == 0) {
    printf("Please enter a choice (1-9): ");
    scanf("%d", &input);
    if(input < 1 || input > 9) {
      printf("Error: input is out of range, try again.\n");
    } else if(board[(input-1)/3][(input-1) % 3] != NONE) {
      printf("Error: that choice was already made.\n");
    } else {
      //we map 1-9 to [0][0] thru [2][2]:
      board[(input-1)/3][(input-1)] = player;
      return;
    }
    input = 0;
  }
}

void randomComputerMove(Move **board) {
  int isValid = 0;
  while(!isValid) {
    int move_row = random() % 3;
    int move_col = random() % 3;
    if(board[move_row][move_col] == NONE) {
      board[move_row][move_col] = O;
      isValid = 1;
    }
  }
  return;
}

void smartComputerMove(Move **board) {


  int row, col;
  Move **copy = copyBoard(board);

  findBestMove(copy, &row, &col);
  board[row][col] = O;
  freeBoard(copy);

  return;
}

void findBestMove(Move **board, int *row, int *col) {

  //always assume computer is O,
  //for each open move:
  int best = -1;
  for(int i=0; i<3; i++) {
    for(int j=0; j<3; j++) {
      if(board[i][j] == NONE) {
        board[i][j] = O;
        //if we can win right away do it
        if(getStatus(board) == O_WINS) {
          *row = i;
          *col = j;
          return;
        }
        int x = numWinningCombos(board, X);
        board[i][j] = NONE;  //undo move
        if(x > best) {
          *row = i;
          *col = j;
          best = x;
        }
      }
    }
  }

}

int numWinningCombos(Move **board, Move nextMove) {

  int total = 0;
  Status s = getStatus(board);
  if(s == PLAYING) {

    for(int i=0; i<3; i++) {
      for(int j=0; j<3; j++) {
        if(board[i][j] == NONE) {
          board[i][j] = nextMove;
          total += numWinningCombos(board, nextMove == X ? O : X);
          board[i][j] = NONE;
        }
      }
    }
    return total;

  } else if(s == O_WINS || s == TIE) {
    //2nd player can never win, only play to a tie
    return 1;
  } else if(s == X_WINS){
    return 0;
  } else {
    printf("Illegal state!\n");
    exit(1);
  }
}
