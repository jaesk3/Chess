#include <stdio.h>
#include "Move.h"
#include "Chess.h"
#define BOARDWIDTH 8
#define BOARDHEIGHT 8

MoveList *ListAvailableMoves( Board *currBoard, bool activePlayer)
{
    const int empty = 0;
    MoveList *availableMoves = CreateMoveList();
    bool rcFlag = true;
    MoveEntry *moveTemp; 
 
    for(int i = 0; i < BOARDWIDTH; i++)
    {
        for(int j = 0; j < BOARDHEIGHT; j++)
        {
            if(currBoard->game_board[i][j]->color == 'w' && activePlayer)//if activePlayer is true, it is white's turn to move
            {
                //en passant
                if(j == 5 && currBoard->game_board[i][5]->type == PAWN)
		{
			if(i+1 < 8 && currBoard->game_board[i+1][5]->color == 'b')
			{
				if(currBoard->game_board[i+1][5]->epSwitch)
				{
					moveTemp = CreateMoveEntry();
					moveTemp->Vector->Startpoint->x = i;
					moveTemp->Vector->Startpoint->y = 5;
					moveTemp->Vector->Endpoint->x = i+1;
					moveTemp->Vector->Endpoint->y = 6;
					AppendMoveEntry(availableMoves, moveTemp);
				}
			}
			
			if(i-1 >= 0 && currBoard->game_board[i-1][5]->color == 'b')
			{
				if(currBoard->game_board[i-1][5]->epSwitch)
				{
					moveTemp = CreateMoveEntry();
					moveTemp->Vector->Startpoint->x = i;
					moveTemp->Vector->Startpoint->y = 5;
					moveTemp->Vector->Endpoint->x = i-1;
					moveTemp->Vector->Endpoint->y = 6;
					AppendMoveEntry(availableMoves, moveTemp);	
				}
			}
		}
		//promotion
                if(j == 7)
		{
			if(currBoard->game_board[i][j]->type == PAWN)
			{
				moveTemp = CreateMoveEntry();
				moveTemp->Vector->Startpoint->x = i;
				moveTemp->Vector->Startpoint->y = j;
				moveTemp->Vector->Endpoint->x = i;
				moveTemp->Vector->Endpoint->y = j;
				moveTemp->Vector->Promotion = true;	
				moveTemp->Vector->AI = true;
				AppendMoveEntry(availableMoves, moveTemp);
			}
		}
		switch(currBoard->game_board[i][j]->type)
                {
                    case PAWN:
                        if(j+1 < 8 && currBoard->game_board[i][j+1]->type == empty)
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i;
                        	moveTemp->Vector->Endpoint->y = j + 1;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(i+1 < 8 && j+1 < 8 && currBoard->game_board[i + 1][j + 1]->type != empty && currBoard->game_board[i + 1][j + 1]->color == 'b')//if the pawn can attack right
                        {
                            MoveEntry *moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i+1;
                            moveTemp->Vector->Endpoint->y = j+1;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(i-1 >= 0 && j+1< 8 && currBoard->game_board[i - 1][j + 1]->type != empty && currBoard->game_board[i - 1][j + 1]->color == 'b') //if the pawn can attack left
                        {
                            MoveEntry *moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i-1;
                            moveTemp->Vector->Endpoint->y = j+1;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!currBoard->game_board[i][j]->hasMoved) //if the pawn hasn't moved
                        {
                            if(j+2 < 8 && currBoard->game_board[i][j + 2]->type == empty && currBoard->game_board[i][j + 1]->type == empty)
                            {
                                moveTemp = CreateMoveEntry();
				moveTemp->Vector->Startpoint->x = i;
                                moveTemp->Vector->Startpoint->y = j;
                                moveTemp->Vector->Endpoint->x = i;
                                moveTemp->Vector->Endpoint->y = j+2;
                                AppendMoveEntry(availableMoves, moveTemp);
                            }
                        }
                        break;
                        
                    case KNIGHT:
                        if(!(i+1 > 7 || j+2 > 7) && !(currBoard->game_board[i+1][j+2]->type != empty && currBoard->game_board[i+1][j+2]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i+1;
                        	moveTemp->Vector->Endpoint->y = j+2;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!(i-1 < 0 || j+2 > 7) && !(currBoard->game_board[i-1][j+2]->type != empty && currBoard->game_board[i-1][j+2]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                        	moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i-1;
                        	moveTemp->Vector->Endpoint->y = j+2;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!(i+1 > 7 || j-2 < 0) && !(currBoard->game_board[i+1][j-2]->type != empty && currBoard->game_board[i+1][j-2]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                        	moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i+1;
                        	moveTemp->Vector->Endpoint->y = j-2;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!(i-1 < 0 || j-2 < 0) && !(currBoard->game_board[i-1][j-2]->type != empty && currBoard->game_board[i-1][j-2]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                        	moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i-1;
                        	moveTemp->Vector->Endpoint->y = j-2;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!(i+2 > 7 || j+1 > 7) && !(currBoard->game_board[i+2][j+1]->type != empty && currBoard->game_board[i+2][j+1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                        	moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i+2;
                        	moveTemp->Vector->Endpoint->y = j+1;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!(i+2 > 7 || j-1 < 0) && !(currBoard->game_board[i+2][j-1]->type != empty && currBoard->game_board[i+2][j-1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                        	moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i+2;
                        	moveTemp->Vector->Endpoint->y = j-1;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!(i-2 < 0 || j+1 > 7) && !(currBoard->game_board[i-2][j+1]->type != empty && currBoard->game_board[i-2][j+1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                        	moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i-2;
                        	moveTemp->Vector->Endpoint->y = j+1;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!(i-2 < 0 || j-1 < 0) && !(currBoard->game_board[i-2][j-1]->type != empty && currBoard->game_board[i-2][j-1]->color == 'w'))
                        {
                        	moveTemp = CreateMoveEntry();
                        	moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i-2;
                        	moveTemp->Vector->Endpoint->y = j-1;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }
                        break;
                        
                    case BISHOP:
                        for(int k = 1; j+k < 8 && i+k < 8; k++)
                        {
                            if(i+k-1 >= 0 && j+k-1 >= 0 && k>1 && currBoard->game_board[i+k-1][j+k-1]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j+k]->type != empty && currBoard->game_board[i+k][j+k]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = -1; j+k >= 0 && i+k >= 0; k--)
                        {
                            if(i+k+1 < 8 && j+k+1 < 8 && k<-1 && currBoard->game_board[i+k+1][j+k+1]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j+k]->type != empty && currBoard->game_board[i+k][j+k]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = 1; i-k >= 0 && j+k < 8; k++)
                        {
                            if(i-k+1 < 8 && j+k-1 >= 0 && k>1 && currBoard->game_board[i-k+1][j+k-1]->type != empty)
                                break;
                            if(currBoard->game_board[i-k][j+k]->type != empty && currBoard->game_board[i-k][j+k]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i - k;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = 1; i+k < 8 && j-k >= 0; k++)
                        {
                            if(i+k-1 >= 0 && j-k+1 < 8 && k>1 && currBoard->game_board[i+k-1][j-k+1]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j-k]->type != empty && currBoard->game_board[i+k][j-k]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j - k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        break;
                        
                    case ROOK:
                        for(int k = 1; i + k < 8; k++)
                        {
                            if(i+k-1 >= 0 && k>1 && currBoard->game_board[i+k-1][j]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j]->type != empty && currBoard->game_board[i+k][j]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = 1; i - k > 0; k++)
                        {
                            if(i-k+1 < 8 && k>1 && currBoard->game_board[i-k+1][j]->type != empty)
                                break;
                            if(currBoard->game_board[i-k][j]->type != empty && currBoard->game_board[i-k][j]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i - k;
                            moveTemp->Vector->Endpoint->y = j;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = 1; j + k < 8; k++)
                        {
                            if(j+k-1 >= 0 && k>1 && currBoard->game_board[i][j+k-1]->type != empty)
                                break;
                            if(currBoard->game_board[i][j+k]->type != empty && currBoard->game_board[i][j+k]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = 1; j - k > 0; k++)
                        {
                            if(j-k+1 < 8 && k>1 && currBoard->game_board[i][j-k+1]->type != empty)
                                break;
                            if(currBoard->game_board[i][j-k]->type != empty && currBoard->game_board[i][j-k]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j - k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!currBoard->game_board[i][j]->hasMoved)//if the rook hasn't moved (castling)
                        {
                            if(!currBoard->game_board[4][0]->hasMoved)//if the king hasn't moved
                            {
                                if(i == 0)
                                {
                                    for(int k = 1; k < 4; k++)
                                    {
                                        if(currBoard->game_board[k][0]->type != empty)//if the pieces are !empty
                                        {
                                            rcFlag = false;
                                            break;
                                        }
                                    }
                                    if(rcFlag)
                                    {
                                    	moveTemp = CreateMoveEntry();
                                    	moveTemp->Vector->Startpoint->x = 0;
                                    	moveTemp->Vector->Startpoint->y = 0;
                                    	moveTemp->Vector->Endpoint->x = 3;
                                    	moveTemp->Vector->Endpoint->y = 0;
                                    	AppendMoveEntry(availableMoves, moveTemp);
                                    }
                                }
                                else
                                {
                                    for(int k = 6; k > 4; k--)
                                    {
                                        if(currBoard->game_board[k][0]->type != empty)//if the piece is empty
                                        {
                                            rcFlag = false;
                                            break;
                                        }
                                    }
		
                                    if(rcFlag)
                                    {
                                    	moveTemp = CreateMoveEntry();
                                    	moveTemp->Vector->Startpoint->x = 7;
                                    	moveTemp->Vector->Startpoint->y = 0;
                                    	moveTemp->Vector->Endpoint->x = 5;
                                    	moveTemp->Vector->Endpoint->y = 0;
                                    	AppendMoveEntry(availableMoves, moveTemp);
                                    }
                                }
                            }
                        }
                        break;
                        
                    case QUEEN:
                        //DIAG_BEGIN
                        for(int k = 1; i+k < 8 && j+k < 8; k++)
                        {
                            if(i+k-1 >= 0 && j+k-1 >= 0 && k<-1 && currBoard->game_board[i+k-1][j+k-1]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j+k]->type != empty && currBoard->game_board[i+k][j+k]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = -1; i+k >= 0 && j+k >= 0; k--)
                        {
                            if(i+k+1 < 8 && j+k+1 < 8 && k<-1 && currBoard->game_board[i+k+1][j+k+1]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j+k]->type != empty && currBoard->game_board[i+k][j+k]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = 1; i-k >= 0 && j+k < 8; k++)
                        {
                            if(i-k+1 < 8 && j+k-1 >= 0 && k>1 && currBoard->game_board[i-k+1][j+k-1]->type != empty)
                                break;
                            if(currBoard->game_board[i-k][j+k]->type != empty && currBoard->game_board[i-k][j+k]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i - k;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = 1; i+k < 8 && j-k >= 0; k++)
                        {
                            if(i+k-1 >= 0 && j-k+1 < 8 && k>1 && currBoard->game_board[i+k-1][j-k+1]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j-k]->type != empty && currBoard->game_board[i+k][j-k]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j - k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }//DIAG_END
                        
                        //FORWARD
                        for(int k = 1; j + k < 8; k++)
                        {
                            if(j+k-1 >= 0 && k>1 && currBoard->game_board[i][j+k-1]->type != empty)
                                break;
                            if(currBoard->game_board[i][j+k]->type != empty && currBoard->game_board[i][j+k]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        
                        //BACKWARD
                        for(int k = -1; j + k > 0; k--)
                        {
                            if(j+k+1 < 8 && k<-1 && currBoard->game_board[i][j+k]->type != empty)
                                break;
                            if(currBoard->game_board[i][j+k]->type != empty && currBoard->game_board[i][j+k]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        
                        //LATERAL_BEGIN
                        for(int k = 1; i + k < 8; k++)
                        {
                            if(i+k-1 >= 0 && k>1 && currBoard->game_board[i+k-1][j]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j]->type != empty && currBoard->game_board[i+k][j]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        
                        for(int k = -1; i + k > 0; k--)
                        {
                            if(i+k+1 < 8 && k<-1 && currBoard->game_board[i+k+1][j]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j]->type != empty && currBoard->game_board[i+k][j]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        //LATERAL_END
                        break;
                        
                    case KING:
                        if(i+1 < 8 && !(currBoard->game_board[i+1][j]->type != empty && currBoard->game_board[i+1][j]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i + 1;
                        	moveTemp->Vector->Endpoint->y = j;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }
                        
                        if(i-1 >= 0 && !(currBoard->game_board[i-1][j]->type != empty && currBoard->game_board[i-1][j]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i - 1;
                        	moveTemp->Vector->Endpoint->y = j;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }
			
                        if(j+1 < 8 && !(currBoard->game_board[i][j+1]->type != empty && currBoard->game_board[i][j+1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                        	moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i;
                        	moveTemp->Vector->Endpoint->y = j + 1;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }
			
                        if(j-1 >= 0 && !(currBoard->game_board[i][j-1]->type != empty && currBoard->game_board[i][j-1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                        	moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i;
                        	moveTemp->Vector->Endpoint->y = j - 1;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }
			
                        if(i+1 < 8 && j+1 < 8 && !(currBoard->game_board[i+1][j+1]->type != empty && currBoard->game_board[i+1][j+1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                        	moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i + 1;
                        	moveTemp->Vector->Endpoint->y = j + 1;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(i+1 < 8 && j-1 >= 0 && !(currBoard->game_board[i+1][j-1]->type != empty && currBoard->game_board[i+1][j-1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                        	moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i + 1;
                        	moveTemp->Vector->Endpoint->y = j - 1;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }
			
                        if(i-1 >= 0 && j+1 < 8 && !(currBoard->game_board[i-1][j+1]->type != empty && currBoard->game_board[i-1][j+1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                        	moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i - 1;
                        	moveTemp->Vector->Endpoint->y = j + 1;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }

                        if(i-1 >= 0 && j-1 >= 0 && !(currBoard->game_board[i-1][j-1]->type != empty && currBoard->game_board[i-1][j-1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                        	moveTemp->Vector->Startpoint->x = i;
                        	moveTemp->Vector->Startpoint->y = j;
                        	moveTemp->Vector->Endpoint->x = i - 1;
                        	moveTemp->Vector->Endpoint->y = j - 1;
                        	AppendMoveEntry(availableMoves, moveTemp);
                        }
                            break;
                        
                    default:
                        break;
                }//hctiws
            }
	    if(j == 4 && currBoard->game_board[i][4]->type == PAWN)
                {
                        if(i+1 < 8 && currBoard->game_board[i+1][4]->color == 'w')
                        {
                                if(currBoard->game_board[i+1][4]->epSwitch)
                                {
                                        moveTemp = CreateMoveEntry();
                                        moveTemp->Vector->Startpoint->x = i;
                                        moveTemp->Vector->Startpoint->y = 4;
                                        moveTemp->Vector->Endpoint->x = i+1;
                                        moveTemp->Vector->Endpoint->y = 3;
                                        AppendMoveEntry(availableMoves, moveTemp);
                                }
                        }

                        if(i-1 >= 0 && currBoard->game_board[i-1][4]->color == 'w')
                        {
                                if(currBoard->game_board[i-1][4]->epSwitch)
                                {
                                        moveTemp = CreateMoveEntry();
                                        moveTemp->Vector->Startpoint->x = i;
                                        moveTemp->Vector->Startpoint->y = 4;
                                        moveTemp->Vector->Endpoint->x = i-1;
                                        moveTemp->Vector->Endpoint->y = 3;
                                        AppendMoveEntry(availableMoves, moveTemp);
                                }
                        }
                }

            //promotion
            if(currBoard->game_board[i][j]->color == 'b' && !activePlayer)//if activePlayer is false, it is black's turn to move
            {
                if(j == 0)
                {
                        if(currBoard->game_board[i][j]->type == PAWN)
                        {
                                moveTemp = CreateMoveEntry();
                                moveTemp->Vector->Startpoint->x = i;
                                moveTemp->Vector->Startpoint->y = j;
                                moveTemp->Vector->Endpoint->x = i;
                                moveTemp->Vector->Endpoint->y = j;
                                moveTemp->Vector->Promotion = true;
                                moveTemp->Vector->AI = true;
                                AppendMoveEntry(availableMoves, moveTemp);
                        }
                }
		switch(currBoard->game_board[i][j]->type)
                {
                    case PAWN:
                        if(j-1 >= 0 && currBoard->game_board[i][j-1]->type == empty)
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j - 1;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(i+1 < 8 && j-1 >= 0 && currBoard->game_board[i + 1][j - 1]->type != empty && currBoard->game_board[i + 1][j - 1]->color == 'w')//if the pawn can attack right
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i+1;
                            moveTemp->Vector->Endpoint->y = j-1;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(i-1 >= 0 && j-1>= 0 && currBoard->game_board[i - 1][j - 1]->type != empty && currBoard->game_board[i - 1][j - 1]->color == 'w') //if the pawn can attack left
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i-1;
                            moveTemp->Vector->Endpoint->y = j-1;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!currBoard->game_board[i][j]->hasMoved) //if the pawn hasn't moved
                        {
                            if(j-2 >= 0 && currBoard->game_board[i][j - 2]->type == empty && currBoard->game_board[i][j - 1]->type == empty)
                            {
                                moveTemp = CreateMoveEntry();
                                moveTemp->Vector->Startpoint->x = i;
                                moveTemp->Vector->Startpoint->y = j;
                                moveTemp->Vector->Endpoint->x = i;
                                moveTemp->Vector->Endpoint->y = j-2;
                                AppendMoveEntry(availableMoves, moveTemp);
                            }
                        }
                        break;
                        
                    case KNIGHT:
                        if(!(i+1 > 7 || j+2 > 7) && !(currBoard->game_board[i+1][j+2]->type != empty && currBoard->game_board[i+1][j+2]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i+1;
                            moveTemp->Vector->Endpoint->y = j+2;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!(i-1 < 0 || j+2 > 7) && !(currBoard->game_board[i-1][j+2]->type != empty && currBoard->game_board[i-1][j+2]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i-1;
                            moveTemp->Vector->Endpoint->y = j+2;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!(i+1 > 7 || j-2 < 0) && !(currBoard->game_board[i+1][j-2]->type != empty && currBoard->game_board[i+1][j-2]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i+1;
                            moveTemp->Vector->Endpoint->y = j-2;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!(i-1 < 0 || j-2 < 0) && !(currBoard->game_board[i-1][j-2]->type != empty && currBoard->game_board[i-1][j-2]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i-1;
                            moveTemp->Vector->Endpoint->y = j-2;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!(i+2 > 7 || j+1 > 7) && !(currBoard->game_board[i+2][j+1]->type != empty && currBoard->game_board[i+2][j+1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i+2;
                            moveTemp->Vector->Endpoint->y = j+1;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!(i+2 > 7 || j-1 < 0) && !(currBoard->game_board[i+2][j-1]->type != empty && currBoard->game_board[i+2][j-1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i+2;
                            moveTemp->Vector->Endpoint->y = j-1;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!(i-2 < 0 || j+1 > 7) && !(currBoard->game_board[i-2][j+1]->type != empty && currBoard->game_board[i-2][j+1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i-2;
                            moveTemp->Vector->Endpoint->y = j+1;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!(i-2 < 0 || j-1 < 0) && !(currBoard->game_board[i-2][j-1]->type != empty && currBoard->game_board[i-2][j-1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i-2;
                            moveTemp->Vector->Endpoint->y = j-1;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        break;
                        
                    case BISHOP:
                        for(int k = 1; i+k < 8 && j+k < 8; k++)
                        {
                            if(i+k-1 >= 0 && j+k-1 >= 0 && k>1 && currBoard->game_board[i+k-1][j+k-1]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j+k]->type != empty && currBoard->game_board[i+k][j+k]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = -1; j+k >= 0 && i+k >= 0; k--)
                        {
                            if(i+k+1 < 8 && j+k+1 < 8 && k<-1 && currBoard->game_board[i+k+1][j+k+1]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j+k]->type != empty && currBoard->game_board[i+k][j+k]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = 1; i-k >= 0 && j+k < 8; k++)
                        {
                            if(i-k+1 < 8 && j+k-1 >= 0 && k>1 && currBoard->game_board[i-k+1][j+k-1]->type != empty)
                                break;
                            if(currBoard->game_board[i-k][j+k]->type != empty && currBoard->game_board[i-k][j+k]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i - k;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = 1; i+k < 8 && j-k >= 0; k++)
                        {
                            if(i+k-1 >= 0 && j-k+1 < 8 && k>1 && currBoard->game_board[i+k-1][j-k+1]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j-k]->type != empty && currBoard->game_board[i+k][j-k]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j - k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        break;
                        
                    case ROOK:
                        for(int k = 1; i + k < 8; k++)
                        {
                            if(i+k-1 >= 0 && k>1 && currBoard->game_board[i+k-1][j]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j]->type != empty && currBoard->game_board[i+k][j]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = 1; i - k > 0; k++)
                        {
                            if(i-k+1 < 8 && k>1 && currBoard->game_board[i-k+1][j]->type != empty)
                                break;
                            if(currBoard->game_board[i-k][j]->type != empty && currBoard->game_board[i-k][j]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i - k;
                            moveTemp->Vector->Endpoint->y = j;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = 1; j + k < 8; k++)
                        {
                            if(j+k-1 >= 0 && k>1 && currBoard->game_board[i][j+k-1]->type != empty)
                                break;
                            if(currBoard->game_board[i][j+k]->type != empty && currBoard->game_board[i][j+k]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = 1; j - k > 0; k++)
                        {
                            if(j-k+1 < 8 && k>1 && currBoard->game_board[i][j-k+1]->type != empty)
                                break;
                            if(currBoard->game_board[i][j-k]->type != empty && currBoard->game_board[i][j-k]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j - k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(!currBoard->game_board[i][j]->hasMoved)//if the rook hasn't moved (castling)
                        {
                            if(!currBoard->game_board[4][7]->hasMoved)//if the king hasn't moved
                            {
                                if(i == 0)
                                {
                                    for(int k = 1; k < 4; k++)
                                    {
                                        if(currBoard->game_board[k][7]->type != empty)//if the pieces are !empty
                                        {
                                            rcFlag = false;
                                            break;
                                        }
                                    }
                                    if(rcFlag)
                                    {
                                        moveTemp = CreateMoveEntry();
                                        moveTemp->Vector->Startpoint->x = 0;
                                        moveTemp->Vector->Startpoint->y = 7;
                                        moveTemp->Vector->Endpoint->x = 3;
                                        moveTemp->Vector->Endpoint->y = 7;
                                        AppendMoveEntry(availableMoves, moveTemp);
                                    }
                                }
                                else
                                {
                                    for(int k = 6; k > 4; k--)
                                    {
                                        if(currBoard->game_board[k][7]->type != empty)//if the piece is empty
                                        {
                                            rcFlag = false;
                                            break;
                                        }
                                    }
                                    
                                    if(rcFlag)
                                    {
                                        moveTemp = CreateMoveEntry();
                                        moveTemp->Vector->Startpoint->x = 7;
                                        moveTemp->Vector->Startpoint->y = 7;
                                        moveTemp->Vector->Endpoint->x = 5;
                                        moveTemp->Vector->Endpoint->y = 7;
                                        AppendMoveEntry(availableMoves, moveTemp);
                                    }
                                }
                            }
                        }
                        break;
                        
                    case QUEEN:
                        //DIAG_BEGIN
                        for(int k = 1; i+k < 8 && j+k < 8; k++)
                        {
                            if(i+k-1 >= 0 && j+k-1 >= 0 && k>1 && currBoard->game_board[i+k-1][j+k-1]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j+k]->type != empty && currBoard->game_board[i+k][j+k]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = -1; j+k >= 0 && i+k >= 0; k--)
                        {
                            if(i+k+1 < 8 && j+k+1 < 8 && k<-1 && currBoard->game_board[i+k+1][j+k+1]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j+k]->type != empty && currBoard->game_board[i+k][j+k]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = 1; i-k >= 0 && j+k < 8; k++)
                        {
                            if(i-k+1 < 8 && j+k-1 >= 0 && k>1 && currBoard->game_board[i-k+1][j+k-1]->type != empty)
                                break;
                            if(currBoard->game_board[i-k][j+k]->type != empty && currBoard->game_board[i-k][j+k]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i - k;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        for(int k = 1; i+k < 8 && j-k >= 0; k++)
                        {
                            if(i+k-1 >= 0 && j-k+1 < 8 && k>1 && currBoard->game_board[i+k-1][j-k+1]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j-k]->type != empty && currBoard->game_board[i+k][j-k]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j - k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }//DIAG_END
                        
                        //FORWARD
                        for(int k = 1; j + k < 8; k++)
                        {
                            if(j+k-1 >= 0 && k>1 && currBoard->game_board[i][j+k-1]->type != empty)
                                break;
                            if(currBoard->game_board[i][j+k]->type != empty && currBoard->game_board[i][j+k]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        
                        //BACKWARD
                        for(int k = -1; j + k > 0; k--)
                        {
                            if(j+k+1 < 8 && k<-1 && currBoard->game_board[i][j+k]->type != empty)
                                break;
                            if(currBoard->game_board[i][j+k]->type != empty && currBoard->game_board[i][j+k]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j + k;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        
                        //LATERAL_BEGIN
                        for(int k = 1; i + k < 8; k++)
                        {
                            if(i+k-1 >= 0 && k>1 && currBoard->game_board[i+k-1][j]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j]->type != empty && currBoard->game_board[i+k][j]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        
                        for(int k = -1; i + k > 0; k--)
                        {
                            if(i+k+1 < 8 && k<-1 && currBoard->game_board[i+k+1][j]->type != empty)
                                break;
                            if(currBoard->game_board[i+k][j]->type != empty && currBoard->game_board[i+k][j]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        //LATERAL_END
                        break;
                        
                    case KING:
                        if(i+1 < 8 && !(currBoard->game_board[i+1][j]->type != empty && currBoard->game_board[i+1][j]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + 1;
                            moveTemp->Vector->Endpoint->y = j;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        
                        if(i-1 >= 0 && !(currBoard->game_board[i-1][j]->type != empty && currBoard->game_board[i-1][j]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i - 1;
                            moveTemp->Vector->Endpoint->y = j;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        
                        if(j+1 < 8 && !(currBoard->game_board[i][j+1]->type != empty && currBoard->game_board[i][j+1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j + 1;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        
                        if(j-1 >= 0 && !(currBoard->game_board[i][j-1]->type != empty && currBoard->game_board[i][j-1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j - 1;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        
                        if(i+1 < 8 && j+1 < 8 && !(currBoard->game_board[i+1][j+1]->type != empty && currBoard->game_board[i+1][j+1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + 1;
                            moveTemp->Vector->Endpoint->y = j + 1;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(i+1 < 8 && j-1 >= 0 && !(currBoard->game_board[i+1][j-1]->type != empty && currBoard->game_board[i+1][j-1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + 1;
                            moveTemp->Vector->Endpoint->y = j - 1;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        
                        if(i-1 >= 0 && j+1 < 8 && !(currBoard->game_board[i-1][j+1]->type != empty && currBoard->game_board[i-1][j+1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i - 1;
                            moveTemp->Vector->Endpoint->y = j + 1;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        
                        if(i-1 >= 0 && j-1 >= 0 && !(currBoard->game_board[i-1][j-1]->type != empty && currBoard->game_board[i-1][j-1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i - 1;
                            moveTemp->Vector->Endpoint->y = j - 1;
                            AppendMoveEntry(availableMoves, moveTemp);
                        }
                        break;
                        
                    default:
                        break;
                }//hctiws
            }
        }
    }
	return availableMoves;
}
