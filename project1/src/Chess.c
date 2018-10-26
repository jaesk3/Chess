#include "Chess.h"
#define OTEST

/* For Piece Structure */
/* ---------------------------------------------------------------------- */

// Allocate
/*
 * Creates new "empty" pieces 
 */
Piece * CreatePiece(void) {
  Piece * piece = malloc(sizeof(Piece));
  piece->type = EMPTY;
  piece->val = EMPTY_VAL;
  piece->color = EMPTY_COLOR;
  piece->hasMoved = false;
  piece->longMove = false;
  piece->epSwitch = false;
  piece->epSwitch1 = false;
  return piece;
}

// Deallocate
/*
 * Destroys chess piece
 */
void DeletePiece(Piece * deleteMe) {
  assert(deleteMe);
  free(deleteMe);
//  deleteMe = NULL;
}

// Mutators
/*
 * Sets the pieces on the board
 */
void SetPiece(int type, int val, char color, bool hasMoved, bool longMove, Piece * piece) {
  piece->type = type;
  piece->val = val;
  piece->color = color;
  piece->hasMoved = hasMoved;
  piece->longMove = longMove;
}

// Accessors/Checks
int GetPieceType(Piece * piece) {
  return piece->type;
}

int GetPieceValue(Piece * piece) {
  return piece->val;
}

char GetPieceColor(Piece * piece) {
  return piece->color;
}

bool HasPieceMoved(Piece * piece) {
  return piece->hasMoved;
}

/* ---------------------------------------------------------------------- */



/* For Player Structure */
/* ---------------------------------------------------------------------- */

// Allocate
Player * CreatePlayer(void) {
  Player * player = malloc(sizeof(Player));
  player->color = WHITE;
  player->comp = false;
  return player;
}

// Deallocate
void DeletePlayer(Player * deleteMe) {
  assert(deleteMe);
  free(deleteMe);
//  deleteMe = NULL;
}

// Mutators
void SetPlayer(char color, bool comp, Player * player) {
  player->color = color;
  player->comp = comp;
}

// Accessors/Checks
char GetPlayerColor(Player * player) {
  return player->color;
}

bool IsComp(Player * player) {
  return player->comp;
}

/* ---------------------------------------------------------------------- */



/* For Gameboard Structure */
/* ---------------------------------------------------------------------- */

// Allocate
Board * CreateBoard(void) {
  Board * board = malloc(sizeof(Board));
  board->Player1 = CreatePlayer();
  board->Player2 = CreatePlayer();
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      board->game_board[x][y] = CreatePiece();
    }
  }
  return board;
}

// Deallocate
void DeleteBoard(Board * deleteMe) {
  assert(deleteMe);
  assert(deleteMe->Player1);
  assert(deleteMe->Player2);
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      assert(deleteMe->game_board[x][y]);
    }
  }
  free(deleteMe->Player1);
  free(deleteMe->Player2);
//  deleteMe->Player1 = NULL;
//  deleteMe->Player2 = NULL;
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      free(deleteMe->game_board[x][y]);
//      deleteMe->game_board[x][y] = NULL;
    }
  }
  free(deleteMe);
//  deleteMe = NULL;
}

// Mutators
void InitializeBoard(Board * board) {
  for(int i = 0; i < 8; i++)
  {
      for(int j = 0; j < 8; j++)
      {
          SetPiece(EMPTY, 0, 'n', false, false, board->game_board[i][j]);
      }
  }
  SetPiece(ROOK, ROOK_VAL, WHITE, false, true, board->game_board[0][0]);
  SetPiece(ROOK, ROOK_VAL, WHITE, false, true, board->game_board[7][0]);
  SetPiece(ROOK, ROOK_VAL * -1, BLACK, false, true, board->game_board[0][7]);
  SetPiece(ROOK, ROOK_VAL * -1, BLACK, false, true, board->game_board[7][7]);
    
  //Knights
  SetPiece(KNIGHT, KNIGHT_VAL, WHITE, false, true, board->game_board[1][0]);
  SetPiece(KNIGHT, KNIGHT_VAL, WHITE, false, true, board->game_board[6][0]);
  SetPiece(KNIGHT, KNIGHT_VAL * -1, BLACK, false, true, board->game_board[1][7]);
  SetPiece(KNIGHT, KNIGHT_VAL * -1, BLACK, false, true, board->game_board[6][7]);
  
  //Bishops
  SetPiece(BISHOP, BISHOP_VAL, WHITE, false, true, board->game_board[2][0]);
  SetPiece(BISHOP, BISHOP_VAL, WHITE, false, true, board->game_board[5][0]);
  SetPiece(BISHOP, BISHOP_VAL * -1, BLACK, false, true, board->game_board[2][7]);
  SetPiece(BISHOP, BISHOP_VAL * -1, BLACK, false, true, board->game_board[5][7]);
  
  //Queens
  SetPiece(QUEEN, QUEEN_VAL, WHITE, false, true, board->game_board[3][0]);
  SetPiece(QUEEN, QUEEN_VAL * -1, BLACK, false, true, board->game_board[3][7]);
  
  //Kings
  SetPiece(KING, KING_VAL, WHITE, false, false, board->game_board[4][0]);
  SetPiece(KING, KING_VAL * -1, BLACK, false, false, board->game_board[4][7]);
  
  //Pawns
  for (int i = 0; i < 8; i++) {
    SetPiece(PAWN, PAWN_VAL, WHITE, false, false, board->game_board[i][1]);
    SetPiece(PAWN, PAWN_VAL * -1, BLACK, false, false, board->game_board[i][6]);
  }
}

// Accessors/Checks
Piece * GetPiece(unsigned char x, unsigned char y, Board * board) {
    return board->game_board[x][y];
}

void CopyBoard(Board *Master, Board *Copy)
{
  	assert(Master);
  	assert(Copy);
  	Piece *MPiece = NULL;
  	Piece *CPiece = NULL;
        for (int i = 0; i < 8; i++) {
      		  for(int j = 0; j < 8; j++) {
          			MPiece = Master->game_board[i][j];
          			CPiece = Copy->game_board[i][j];
          			CPiece->type = MPiece->type;
          			CPiece->color = MPiece->color;
          			CPiece->val = MPiece->val;
          			CPiece->hasMoved = MPiece->hasMoved;
          			CPiece->longMove = MPiece->longMove;
          			CPiece->epSwitch = MPiece->epSwitch;
          			CPiece->epSwitch1 = MPiece->epSwitch1;
          	}
      	}
  }
/* ---------------------------------------------------------------------- */

BoardList *CreateBoardList()
{
	BoardList *newList = malloc(sizeof(BoardList));
	newList->First = NULL;
	newList->Last = NULL;
	newList->ListLength = 0;
	return newList;	
}

void AppendBoard(BoardList *list, Board *entry)
{
	if(list->First == NULL)//Empty List
	{
		list->First = entry;
		list->Last = entry;
		entry->Next = NULL;
		entry->Prev = NULL;
	}
	else
	{
		list->Last->Next = entry;
		entry->Prev = list->Last;
		list->Last = entry;
		entry->Next = NULL;		
	}
	list->ListLength++;
}

void DeleteLastBoard(BoardList *list)
{
	if(list->Last == NULL)
		return;
	if(list->First == NULL)
		return;
	Board *temp = list->Last->Prev;
	DeleteBoard(list->Last);
	list->Last = temp;
	if (temp)
		temp->Next = NULL;
}

void DeleteBoardList(BoardList *list)
{
	for(int i = 0; i < list->ListLength; i++)
	{
		if(list->Last == NULL)
			break;	
		DeleteLastBoard(list);			
	}
	free(list);
}






























