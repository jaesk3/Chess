#ifndef CHESS_H
#define CHESS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

//piece type key
#define EMPTY 0
#define PAWN 1
#define KNIGHT 2
#define BISHOP 3
#define ROOK 4
#define KING 5
#define QUEEN 6

// key for minimax values
#define EMPTY_VAL 0
#define PAWN_VAL 10
#define KNIGHT_VAL 30
#define BISHOP_VAL 30
#define ROOK_VAL 50
#define KING_VAL 900
#define QUEEN_VAL 90

// color key
#define WHITE 'w'
#define BLACK 'b'
#define EMPTY_COLOR 'e'



/* For Piece Structure */
/* ---------------------------------------------------------------------- */
typedef struct piece {
	int type;
	int val;
	char color;
	bool hasMoved;
	bool longMove;
	bool epSwitch;
	bool epSwitch1;
} Piece; 

// Allocate
Piece * CreatePiece(void);

// Deallocate
void DeletePiece(Piece * deleteMe);

// Mutators
void SetPiece(int type, int val, char color, bool hasMoved, bool longMove, Piece * piece);

// Accessors/Checks
int GetPieceType(Piece * piece);
int GetPieceValue(Piece * piece);
char GetPieceColor(Piece * piece);
bool HasPieceMoved(Piece * piece);

/* ---------------------------------------------------------------------- */



/* For Player Structure */
/* ---------------------------------------------------------------------- */
typedef struct player {
	bool color;
	char comp;
	// bool active;
} Player;

// Allocate
Player * CreatePlayer(void);

// Deallocate
void DeletePlayer(Player * deleteMe);

// Mutators
void SetPlayer(char color, bool comp, Player * player);

// Accessors/Checks
char GetPlayrerColor(Player * player);
bool IsComp(Player * player);

/* ---------------------------------------------------------------------- */



/* For Gameboard Structure */
/* ---------------------------------------------------------------------- */
typedef struct gameboard Board; 

struct gameboard {
	Player * Player1;
	Player * Player2;
	Piece * game_board [8][8];
	bool currPlayer;
	Board *Next;
	Board *Prev;
};

typedef struct {
	int ListLength;
	Board *First;
	Board *Last;
} BoardList;

// Allocate
Board * CreateBoard(void);

// Deallocate
void DeleteBoard(Board * deleteMe);

// Mutators
void InitializeBoard(Board * board);

// Accessors/Checks
Piece * GetPiece(unsigned char x, unsigned char y, Board * board);

// Functions for Minimax

unsigned char FindKing(Board *currBoard, bool activePlayer);

void CopyBoard(Board *currBoard, Board *newBoard);
/* ---------------------------------------------------------------------- */

BoardList *CreateBoardList(void);

void AppendBoard(BoardList *list, Board *entry);

void DeleteLastBoard(BoardList *list);

void DeleteBoardList(BoardList *list);

#endif
