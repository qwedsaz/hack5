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
