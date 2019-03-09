#include<stdlib.h>
#include<stdio.h>
#include<time.h>


typedef enum {
  TWO_PLAYER        = 1,
  ONE_PLAYER_RANDOM = 2,
  ONE_PLAYER_SMART  = 3,
} GameMode;

typedef enum {
  NONE,
  O,
  X,
} Move;

typedef enum {
  PLAYING,
  O_WINS,
  X_WINS,
  TIE,
} Status;

/**
 * Create a new tic-tac-toe board with all entries initialized
 * to NONE
 */
Move ** newBoard();

/**
 * Creates a deep copy of the given tic-tac-toe board
 */
Move ** copyBoard(Move **board);

/**
 * Frees up the given board
 */
void freeBoard(Move **board);

/**
 * Prints the given game board to the standard output
 */
void printBoard(Move **board);

/**
 * Prompts the user to choose a given game mode, returning
 * their choice.
 */
GameMode mainMenu();

/**
 * Determines the status of the given board, i.e. if it
 * represents a winning configuration for X, O, or its a tie
 * or that play should continue.
 */
Status getStatus(Move **board);

/**
 * Prompts the (human) player for a valid move and
 * updates the game board.  Which player (X or O)
 * is indicated by the given player.
 */
void userMove(Move **board, Move player);

/**
 * Makes a random move on the given board
 * The computer player is always assumed to be O
 */
void randomComputerMove(Move **board);

/**
 * Given the current board along with who plays next
 * (X or O), determines the number of "winning" combinations
 * recursively.  Actually returns the number of wins/ties
 * since as the second player, a computer player can only
 * play to a tie game.
 */
int numWinningCombos(Move **board, Move nextMove);

/**
 * Given the current board, the computer plays the next
 * best (ie least worst) move.
 */
void smartComputerMove(Move **board);

/**
 * Determines the the best row/column combination to play
 * next given the current board.
 */
void findBestMove(Move **board, int *row, int *col);

int main(int argc, char **argv) {

  srandom(time(NULL));

  int gameChoice = mainMenu();
  Move **board = newBoard();
  Status s = getStatus(board);

  while(s == PLAYING) {

    //x moves
    printBoard(board);
    userMove(board, X);
    s = getStatus(board);
    if(s != PLAYING) {
      break;
    }

    //o moves
    printBoard(board);
    if(gameChoice == TWO_PLAYER) {
      userMove(board, O);
    } else if(gameChoice == ONE_PLAYER_RANDOM) {
      randomComputerMove(board);
    } else if(gameChoice == ONE_PLAYER_SMART) {
      smartComputerMove(board);
    }
    s = getStatus(board);

  }

  printf("Final Board:\n");
  printBoard(board);

  if(s == O_WINS) {
    printf("O Wins!\n");
  } else if(s == X_WINS) {
    printf("X Wins!\n");
  } else if(s == TIE) {
    printf("Cats game, please play again!\n");
  }

  return 0;
}

void freeBoard(Move **board) {

  int i;
  for(i=0; i<3; i++) {
    free(board[i]);
  }
  free(board);
  return;
}

Move ** copyBoard(Move **board) {

  Move **copy = newBoard();
  int i, j;
  for(i=0; i<3; i++) {
    for(j=0; j<3; j++) {
      copy[i][j] = board[i][j];
    }
  }
  return board;
}

Move ** newBoard() {
  Move **board = (Move **) malloc(3 * sizeof(Move*));
  int i;
  for(i=0; i<3; i++) {
    board[i] = (Move *) malloc(3 * sizeof(Move));
    int j;
    for(j=0; j<3; j++) {
      board[i][j] = NONE;
    }
  }
  return board;
}

void printBoard(Move **board) {

  printf("\n");
  int i;
  for(i=0; i<3; i++) {
    printf("  ");
    int j;
    for(j=0; j<3; j++) {
      if(board[i][j] == X) {
        printf("  X  ");
      } else if(board[i][j] == O) {
        printf("  O  ");
      } else {
        printf(" (%d) ", (i*3+j+1) );
      }
      if(j < 2) {
        printf(" | ");
      }
    }
    printf("\n");
    if(i<3) {
      printf("  ------+-------+------\n");
    }
  }
  printf("\n");

  return;
}

GameMode mainMenu() {

  int choice = 0;
  while(choice < 1 || choice > 3) {
    printf("Welcome to Tic-Tac-Toe!!\n");
    printf("========================\n");
    printf("Menu\n");
    printf("(1) 2-player game\n");
    printf("(2) 1-player game vs random computer\n");
    printf("(3) 1-player game vs smart computer\n");
    scanf("%d", &choice);
  }
  return choice;
}


Status getStatus(Move **board) {

  Status s = TIE;

  if( //rows
      (board[0][0] == board[0][1] && board[0][1] == board[0][2] && board[0][0] == X) ||
      (board[1][0] == board[1][1] && board[1][1] == board[1][2] && board[1][0] == X) ||
      (board[2][0] == board[2][1] && board[2][1] == board[2][2] && board[2][0] == X) ||
      //cols
      (board[0][0] == board[1][0] && board[1][0] == board[2][0] && board[0][0] == X) ||
      (board[0][1] == board[1][1] && board[1][1] == board[2][1] && board[0][1] == X) ||
      (board[0][2] == board[1][2] && board[1][2] == board[2][2] && board[0][2] == X) ||
      //across
      (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] == X) ||
      (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] == X)  ) {
    s = X_WINS;
  } else if(
      //rows
      (board[0][0] == board[0][1] && board[0][1] == board[0][2] && board[0][0] == O) ||
      (board[1][0] == board[1][1] && board[1][1] == board[1][2] && board[1][0] == O) ||
      (board[2][0] == board[2][1] && board[2][1] == board[2][2] && board[2][0] == O) ||
      //cols
      (board[0][0] == board[1][0] && board[1][0] == board[2][0] && board[0][0] == O) ||
      (board[0][1] == board[1][1] && board[1][1] == board[2][1] && board[0][1] == O) ||
      (board[0][2] == board[1][2] && board[1][2] == board[2][2] && board[0][2] == O) ||
      //across
      (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] == O) ||
      (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] == O)  ) {
    s = O_WINS;
  } else {

    //if there are any unplayed squares, keep playing
    int i, j;
    for(i=0; i<3; i++) {
      for(j=0; j<3; j++) {
        if(board[i][j] == NONE) {
          s = PLAYING;
          return s;
        }
      }
    }
  }
  return s;
}

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
      board[(input-1)/3][((input-1) % 3)] = player;
      return;
    }
    input = 0;
  }
}

void randomComputerMove(Move **board) {
  int isValid = 0;
  while(!isValid) {
    int move_row = rand() % 3;
    int move_col = rand() % 3;
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
  printf("%u\n%u\n", board[0][0], copy[0][0]);
  freeBoard(copy);

  return;
}

void findBestMove(Move **board, int *row, int *col) {

  //always assume computer is O,
  //for each open move:
  int best = -1;
  int i;
  for(i=0; i<3; i++) {
    int j;
    for(j=0; j<3; j++) {
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
        if(x > best && board[i][j] != X) {
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

    int i;
    for(i=0; i<3; i++) {
      int j;
      for(j=0; j<3; j++) {
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
