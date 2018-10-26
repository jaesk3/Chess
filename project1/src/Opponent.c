//
//  Opponent.c
//  
//
//  Created by Kingsmen 
//

#include "Opponent.h"

//Figures out if it is opening
bool IsOpener(Board *currBoard)
{
    static int moveCount = 0;
    moveCount++;
    if(moveCount > 2)
        return false;
    else
        return true;
}

//Special case for opening move
MoveEntry *GiveOpeningMove();


int minimax(Board *currBoard, int depth, bool isMaxPlayer, int alpha, int beta)
{
    #ifdef DEBUG
    static int numTimes = 0;
    #endif  
    if(depth == 0)
    {
        #ifdef DEBUG
	numTimes++;
	#endif
	return FindTotalScore(currBoard, isMaxPlayer);
    }    

    int bestMove;
    int testMove;
    Board *testBoard = CreateBoard();
    MoveList *possibleMoves = ListAvailableMoves(currBoard, isMaxPlayer);
    MoveEntry *currMove = possibleMoves->First;
    
    CopyBoard(currBoard, testBoard);   
 
    if(isMaxPlayer)
    {
        bestMove = -10000;
        for(int i = 0; i < possibleMoves->ListLength; i++)
        {
       	    UpdateBoard(testBoard, currMove);
	    testMove = minimax(testBoard, depth-1, false, alpha, beta);
            bestMove = bestMove > testMove ? bestMove : testMove;
	    //Undo
            CopyBoard(currBoard, testBoard);
	    alpha = alpha > bestMove ? alpha : bestMove;
	    if(currMove)
		currMove = currMove->Next;
	    if(testMove > bestMove)
                bestMove = testMove;
            if(currMove == NULL)
            {
          	break;
            }
	    if (beta <= alpha)
	   	break;
        }
	DeleteBoard(testBoard);
	DeleteMoveList(possibleMoves);
	#ifdef DEBUG
        numTimes++;
	#endif
        return bestMove;
    }
    
    else
    {
        bestMove = 10000;
        for(int i = 0; i < possibleMoves->ListLength; i++)
        {
            UpdateBoard(testBoard, currMove);
	    testMove = minimax(testBoard, depth-1, true, alpha, beta); 
            bestMove = bestMove > testMove ? bestMove : testMove;
	    //Undo
            CopyBoard(currBoard, testBoard);
            beta = beta < bestMove ? beta : bestMove;
	    if(currMove)
		currMove = currMove->Next;
            if(testMove < bestMove)
                bestMove = testMove;
            if(currMove == NULL)
           	break; 
	    if(beta <= alpha)
		break;
        }
	DeleteBoard(testBoard);
	DeleteMoveList(possibleMoves);
	#ifdef DEBUG
	numTimes++;
	#endif
    	return bestMove;
    }
}


#ifdef DEBUG
void DisplayBoardTest(Board *GameBoard) {

        char DisplayingBoard[41][8];

        for (int j = 7; j >= 0; j--) {
                for (int i = 7; i >= 0; i--) {

                        DisplayingBoard[5 * i][j] = '|';
                        DisplayingBoard[(5*i)+1][j] = ' ';

                        switch (GameBoard->game_board[i][j]->type) {
                                case PAWN:
                                        DisplayingBoard[(5*i)+2][j] = GameBoard->game_board[i][j]->color;
                                        DisplayingBoard[(5*i)+3][j] = 'P';
                                        DisplayingBoard[(5*i)+4][j] = ' ';

                                        break;
                                case ROOK:
                                        DisplayingBoard[(5*i)+2][j] = GameBoard->game_board[i][j]->color;
                                        DisplayingBoard[(5*i)+3][j] = 'R';
                                        DisplayingBoard[(5*i)+4][j] = ' ';

                                        break;
                                case KNIGHT:
                                        DisplayingBoard[(5*i)+2][j] = GameBoard->game_board[i][j]->color;
                                        DisplayingBoard[(5*i)+3][j] = 'N';
                                        DisplayingBoard[(5*i)+4][j] = ' ';

                                        break;
                                case BISHOP:
                                        DisplayingBoard[(5*i)+2][j] = GameBoard->game_board[i][j]->color;
                                        DisplayingBoard[(5*i)+3][j] = 'B';
                                        DisplayingBoard[(5*i)+4][j] = ' ';

                                        break;
                                case QUEEN:
                                        DisplayingBoard[(5*i)+2][j] = GameBoard->game_board[i][j]->color;
                                        DisplayingBoard[(5*i)+3][j] = 'Q';
                                        DisplayingBoard[(5*i)+4][j] = ' ';
				case KING:
                                        DisplayingBoard[(5*i)+2][j] = GameBoard->game_board[i][j]->color;
                                        DisplayingBoard[(5*i)+3][j] = 'K';
                                        DisplayingBoard[(5*i)+4][j] = ' ';

                                        break;
                                default:
                                        DisplayingBoard[(5*i)+2][j] = ' ';
                                        DisplayingBoard[(5*i)+3][j] = ' ';
                                        DisplayingBoard[(5*i)+4][j] = ' ';

                                        break;
                        }
                }
                DisplayingBoard[40][j] = '|';
        }

        for (int j = 7; j >= 0; j--) {

                PBorder();
                printf("%d ", 8 - j);

                for (int i = 0; i < 41; i++) {
                        printf("%c", DisplayingBoard[i][j]);
                }
                printf("\n");
        }

        PBorder();
        printf("    a    b    c    d    e    f    g    h    \n");

}

void PBorder(void){
        printf("  +----+----+----+----+----+----+----+----+\n");
}
#endif


MoveEntry *FindBestMove(Board *currBoard, bool activePlayer, unsigned char depth)
{
	Board *testBoard = CreateBoard();   	
        CopyBoard(currBoard, testBoard);
	MoveList *availableMoves = ListAvailableMoves(currBoard, activePlayer);
	bool check = false;
	int bestMove = 9999;
	int tryMove = 0;
	if(activePlayer)
		bestMove = -9999;
	MoveEntry *bestPosTemp = NULL;
	MoveEntry *bestPos = CreateMoveEntry();
	MoveEntry *currMove = availableMoves->First;
	for(int i = 0; i < availableMoves->ListLength; i++)
	{
		check = false;
		if(currMove == NULL)
			break;
		UpdateBoard(testBoard, currMove);       
		tryMove = minimax(testBoard, depth, !activePlayer, -9999, 9999); 
		check = Check(testBoard, activePlayer);
		CopyBoard(currBoard, testBoard);
		if(tryMove > bestMove && activePlayer)
		{
			if(!check)
			{
				bestMove = tryMove;
				bestPosTemp = currMove;
			}
		}			
		else if(tryMove < bestMove && !activePlayer)
		{
			if(!check)
			{
				bestMove = tryMove;
				bestPosTemp = currMove;
			}
		}	
		currMove = currMove->Next;			
	}
	bestPos->Vector->Startpoint->x = bestPosTemp->Vector->Startpoint->x;
	bestPos->Vector->Startpoint->y = bestPosTemp->Vector->Startpoint->y;
	bestPos->Vector->Endpoint->x = bestPosTemp->Vector->Endpoint->x;
	bestPos->Vector->Endpoint->y = bestPosTemp->Vector->Endpoint->y;
	bestPos->Vector->Piecetype = bestPosTemp->Vector->Piecetype;
	bestPos->Vector->Player = bestPosTemp->Vector->Player;
	bestPos->Vector->Promotion = bestPosTemp->Vector->Promotion;
	bestPos->Vector->EnPassant = bestPosTemp->Vector->EnPassant;
	bestPos->Vector->Castling = bestPosTemp->Vector->Castling;
	bestPos->Vector->AI = true;
	if(bestPos == NULL)
		bestPos = availableMoves->First;
	DeleteMoveList(availableMoves);
	DeleteBoard(testBoard);
	return bestPos;	
}


const int WBoardMask[8][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,1,1,0,0,0},{1,1,1,3,3,1,1,1},{1,1,1,3,3,1,1,1},{0,0,0,1,1,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};

const int BBoardMask[8][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,-1,-1,0,0,0},{-1,-1,-1,-3,-3,-1,-1,-1},{-1,-1,-1,-3,-3,-1,-1,-1},{0,0,0,-1,-1,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};

const int PawnMask[8][8] = {{-10,-10,-10,-10,-10,-10,-10,-10},{-1,-1,-1,-1,-1,-1,-1,-1},{0,0,0,0,0,0,0,0},{1,1,1,2,2,1,1,1},{2,2,2,3,3,2,2,2},{3,3,3,4,4,3,3,3},{5,5,5,5,5,5,5,5},{10,10,10,10,10,10,10,10}};

const int KingMask[8][8] = {{3,3,3,3,3,3,3,3},{2,2,2,2,2,2,2,2},{0,0,0,0,0,0,0,0},{-1,-1,-1,-1,-1,-1,-1,-1},{-3,-3,-3,-3,-3,-3,-3,-3},{-5,-5,-5,-5,-5,-5,-5,-5},{-8,-8,-8,-8,-8,-8,-8,-8},{-10,-10,-10,-10,-10,-10,-10,-10}};



// Functions for Minimax
int FindTotalScore(Board * board, bool activePlayer) {
    int sum = 0;
        for(int x = 0; x < 8; x++)
        {
                for(int y = 0; y < 8; y++)
                {
                        sum += board->game_board[x][y]->val;
                        if(board->game_board[x][y]->type != 0 && board->game_board[x][y]->color == 'b')
                                sum += BBoardMask[x][y];
                        else if(board->game_board[x][y]->type != 0 && board->game_board[x][y]->color == 'w')
                                sum += WBoardMask[x][y];
                }
        }
     return sum;
}








