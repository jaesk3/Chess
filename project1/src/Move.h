//
//  move.h
//  chess
//
//  Created by Chang Chi Feng on 17/1/2018.
//  Copyright © 2018年 Chang Chi Feng. All rights reserved.
//

#ifndef move_h
#define move_h
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#include "Chess.h"

typedef struct MoveList   MoveList;
typedef struct MoveEntry  MoveEntry;
typedef struct MoveVector MoveVector;
typedef struct ChessGame  Game;
typedef struct Vector     Vector;

bool IsValidMove(int x1, int y1, int x2, int y2, Board *CurrBoard);

//normal moves basis
bool King(int x1, int y1, int x2, int y2, Board *CurrBoard);
bool Pawn(int x1, int y1, int x2, int y2, Board *CurrBoard);
bool Knight(int x1, int y1, int x2, int y2, Board *CurrBoard);
bool Bishop(int x1, int y1, int x2, int y2, Board *CurrBoard);
bool Rook(int x1, int y1, int x2, int y2, Board *CurrBoard);
bool Queen(int x1, int y1, int x2, int y2, Board *CurrBoard);

//special cases
bool Capture(int x1, int y1, int x2, int y2, Board *CurrBoard);
bool Block(int x1, int y1, int x2, int y2, Board *CurrBoard);
bool OutofBoard(MoveVector *vector);
bool isGameOver(Board *CurrBoard);
bool Promotion(int x1, int y1, int x2, int y2, Board *currBoard);
bool EnPassant(int x1,int y1,int x2,int y2, Board* currBoard);
bool CheckMate(Board* currBoard, bool activePlayer);
bool Check(Board* currBoard, bool activePlayer);
bool RepetitiveMoveThree(MoveList* list);
bool Castling(int x1,int y1,int x2,int y2, Board *currBoard);

//MoveList
MoveList *CreateMoveList(void);

//MoveEntry
MoveEntry *NewMoveEntry(MoveVector *v);
MoveEntry *CreateMoveEntry(void);
MoveEntry *DeleteLastEntry(MoveList* list);
void AppendMove(MoveList *list, MoveVector *vector);
void AppendMoveEntry(MoveList *list, MoveEntry *e);

//MoveVector
MoveVector *NewMoveVector(Board *CurrBoard, int x1, int y1, int x2, int y2);

//Delete
void DeleteMoveList(MoveList *list);
void DeleteMoveVector(MoveVector *v);
MoveVector *DeleteMoveEntry(MoveEntry* e);

void UpdateBoard(Board *currBoard, MoveEntry *currMove);

//MoveEntry *CreateMoveEntry(MoveVector *vector);
//MoveEntry* AppendMove(MoveList *list, MoveVector *vector);
//AI part
MoveList *ListAvailableMoves(Board *currBoard, bool activePlayer);
//enum {king, queen, rook, bishop, knight, pawn} Piecetype;

//Structs
struct MoveVector{
    int Piecetype;
    char Player;
    Vector *Startpoint;
    Vector *Endpoint;
    bool Capture;
    bool Castling;
    bool EnPassant;
    bool Promotion;
    bool AI;
};

struct MoveEntry{
    MoveEntry  *Next;
    MoveEntry  *Prev;
    MoveVector *Vector;
};

struct MoveList{
    unsigned long ListLength;
    MoveEntry *First;
    MoveEntry *Last;
};
struct Vector{
    int x;   // x and y position
    int y;
};

struct Game{
    MoveList *NewList;
};




#endif /* move_h */
