//
//  Opponent.h
//  
//
// Created by Kingsmen 
//

#ifndef Opponent_h
#define Opponent_h
#ifndef DEBUG
#include <stdlib.h>
#include <time.h>
#endif
#include "Chess.h"
#include "Move.h"
#define EASY 2
#define MEDIUM 5
#define HARD 8

//Helper function to findBestMove()
int minimax(Board *currBoard, int depth, bool isMaxPlayer, int alpha, int beta);

//Calculates the best move to a certain depth using the minimax helper function
MoveEntry *FindBestMove(Board *currBoard, bool activePlayer, unsigned char depth);

//Figures out if it is opening
bool IsOpener(Board *currBoard);

//Special case for opening move
MoveEntry *GiveOpeningMoveOpeningMove();

#ifdef DEBUG
void DisplayBoard(Board *currboard);

void PBorder(void);
#endif

int FindTotalScore(Board *currBoard, bool activePlayer);

#endif /* Opponent_h */
