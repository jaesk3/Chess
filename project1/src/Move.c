//
//  move.c
//  chess
//
//  Created by Chang Chi Feng on 19/1/2018.
//  Copyright © 2018年 Chang Chi Feng. All rights reserved.
//

#include "Move.h"
#define BOARDWIDTH 8
#define BOARDHEIGHT 8
#define empty 0
#define pawn 1
#define knight 2
#define bishop 3
#define rook 4
#define king 5
#define queen 6
/* for my reference only
 typedef struct player {
 bool color;
 char comp;
 // bool active;
 } PLAYER;
typedef struct piece {
    int type;
    int val;
    char color;
    bool hasMoved;
    bool longMove;
} PIECE;
 typedef struct gameboard {
 PLAYER * player1;
 PLAYER * player2;
 PIECE * game_board [8][8];
 } BOARD;
 */

//rule
bool Capture(int x1, int y1, int x2, int y2, Board *CurrBoard){
    char activePlayer = CurrBoard->game_board[x1][y1]->color;
    
    //check destination if there's piece within valid move
    if (CurrBoard->game_board[x2][y2] != 0 && CurrBoard->game_board[x2][y2]->color != activePlayer  && IsValidMove(x1,x2,y1,y2,CurrBoard) == 1){
        //needa add check the destination piece is the opposite player (either 1 or 0)
        return true;
    }else{
        return false;
    }
}



bool isGameOver(Board *CurrBoard){
// first condition, king is dead
    int kingCounter = 0;
    for(int i = 0; i < BOARDWIDTH; i++){
        for(int j = 0; j < BOARDHEIGHT; j++){
            if(CurrBoard->game_board[i][j]->type == king){
                kingCounter++;
            }
        }
    }
    if (kingCounter < 2){
        return true;
    }else{
        return false;
    }
}

MoveList *CreateMoveList(void){
    MoveList* list;
    list = malloc(sizeof(MoveList));
    if (! list)
        { perror("Out of memory! Aborting...");
            exit(10);
        }
    list->First = NULL;
    list->Last = NULL;
    list->ListLength = 0; 
    return list;
}
void DeleteMoveList(MoveList *list){
    assert(list);
    MoveEntry *e, *n;
    MoveVector *vector1;
        assert(list);
        e = list->First;
        while(e)
        { 
	    n = e->Next;
            vector1 = DeleteMoveEntry(e);
            DeleteMoveVector(vector1);
            e = n;
        }
        free(list);
}

MoveVector *DeleteMoveEntry(MoveEntry* e){
    MoveVector *v;
    assert(e);
    v = e->Vector;
    free(e);
    return v;
}

MoveEntry *DeleteLastEntry(MoveList* list){
    MoveEntry *temp;
    temp = list->Last;
    list->Last = temp->Prev;
    DeleteMoveEntry(temp); 
    temp = NULL;
    list->ListLength--;
    if(list->Last)
	list->Last->Next = NULL;
    return list->Last;
}

void DeleteMoveVector(MoveVector *v){
    assert(v);
    free(v->Startpoint);
    free(v->Endpoint);
    free(v);
}
MoveVector *NewMoveVector(Board *CurrBoard, int x1, int y1, int x2, int y2){
    if(IsValidMove(x1,y1,x2,y2, CurrBoard) == 1){
        MoveVector *v;
        v = malloc(sizeof(MoveVector));
        assert(v);  
       
	v->Startpoint = malloc(sizeof(Vector));
	v->Endpoint = malloc(sizeof(Vector));
 
        v->Startpoint->x = x1;
        v->Startpoint->y = y1;
        
        v->Endpoint->x = x2;
        v->Endpoint->y = y2;
        
        v->Capture = Capture(x1, y1, x2, y2, CurrBoard);
        v->EnPassant = EnPassant(x1,y1,x2,y2,CurrBoard);
        v->Promotion = Promotion (x1,y2,x2,y2,CurrBoard);
        v->Castling = Castling(x1,y1,x2,y2,CurrBoard);
	v->AI = false;
        
        return v;
        
        }
            else{
                return NULL;
            }
}



MoveEntry *NewMoveEntry(MoveVector *v){
    MoveEntry *e;
    e = malloc(sizeof(MoveEntry));
    if (! e)
    { perror("Out of memory! Aborting...");
        exit(10);
    }
    e->Next = NULL;
    e->Prev = NULL;
    e->Vector = v;
    return e;
}
MoveEntry *CreateMoveEntry (void){
    MoveEntry *e;
    e = malloc(sizeof(MoveEntry));
    if (! e)
    { perror("Out of memory! Aborting...");
        exit(10);
    }
    e->Next = NULL;
    e->Prev = NULL;
    e->Vector = malloc(sizeof(MoveVector));
    e->Vector->Startpoint = malloc(sizeof(Vector));
    e->Vector->Endpoint = malloc(sizeof(Vector));
    e->Vector->Castling = false;
    e->Vector->Promotion = false;
    e->Vector->EnPassant = false;
    e->Vector->AI = true;
    return e;
}

void AppendMove(MoveList *list, MoveVector *vector){
    
    assert(list);
    assert(vector);
    MoveEntry *e;
    e = malloc(sizeof(MoveEntry));
    if (! e)
    { perror("Out of memory! Aborting...");
        exit(10);
    }
    if (list->First == NULL){
        list->First = e;
        list->Last = e;
        e->Next = NULL;
        e->Prev = NULL;
        e->Vector = vector;
        }
        else{
        e->Next = NULL;
        e->Prev = list->Last;
        e->Vector = vector;
        if(list->Last != NULL)
		list->Last->Next = e;
        list->Last = e; 
    }
    list->ListLength++;
    
}

bool IsValidMove(int x1, int y1, int x2, int y2, Board *CurrBoard){
    int type = CurrBoard->game_board[x1][y1] ->type;
    switch (type){
        case empty :
            return 0;
        case pawn :
            if(Pawn(x1, y1, x2, y2, CurrBoard) == 1){
                if(Block(x1,y1,x2,y2,CurrBoard) != 1){
                    return 1;
                }
                else{
                    return 0;
                }
            }else{
                return 0;
            }
        case knight :
            if(Knight(x1, y1, x2, y2, CurrBoard) == 1){
                if(Block(x1,y1,x2,y2,CurrBoard) != 1){
                    return 1;
                }
                else{
                    return 0;
                }
            }else{
                return 0;
            }
        case bishop :
            if(Bishop(x1, y1, x2, y2, CurrBoard) == 1){
                if(Block(x1,y1,x2,y2,CurrBoard) != 1){
                    return 1;
                }
                else{
                    return 0;
                }
            }else{
                return 0;
            }
        case rook :
            if(Rook(x1, y1, x2, y2, CurrBoard) == 1){
                if(Block(x1,y1,x2,y2,CurrBoard) != 1){
                    return 1;
                }
                else{
                    return 0;
                }
            }else{
                return 0;
            }
        case king :
            if(King(x1, y1, x2, y2,CurrBoard) == 1){
                if(Block(x1,y1,x2,y2,CurrBoard)!= 1){
                    return 1;
                }
                else{
                    return 0;
                }
            }else{
                return 0;
            }
        case queen :
            if(Queen(x1, y1, x2, y2, CurrBoard) == 1){
                if(Block(x1,y1,x2,y2,CurrBoard) != 1){
                    return 1;
                }
                else{
                    return 0;
                }
            }else{
                return 0;
            }
        default:
            exit(0);
    }
}

bool King(int x1, int y1, int x2, int y2, Board *CurrBoard){
    if( (abs(x2-x1)==1 && abs(y2-y1)<= 1) || (abs(y2-y1)==1 && abs(x2-x1)<=1)){
        return true;}
    else{
        return false;
    }
}

bool Pawn(int x1, int y1, int x2, int y2, Board* currBoard){
    char activePlayer = currBoard->game_board[x1][y1]->color;
    if(EnPassant(x1, y1, x2, y2, currBoard)){
        return true;
     }
    if(activePlayer == 'w'){
        if (abs(x2-x1) == 1 && GetPieceColor(currBoard->game_board[x2][y2]) == 'b' && y2>y1 && abs(y2-y1) == 1){
            return true;
        }
        if(x1 == x2 && y2>y1){
            if(abs(y2-y1) == 1){
                return true;
            }
            else if(abs(y2-y1)==2 && y1 == 1){
                return true;
            }else{
                return false;
            }
        }
        return false;
    }
    if(activePlayer == 'b'){
        if (abs(x2-x1) == 1 && GetPieceColor(currBoard->game_board[x2][y2]) == 'w' && y1>y2 && abs(y2-y1) == 1 ){
            return true;
        }
        if(x1 == x2 && y1>y2){
            if(abs(y2-y1) == 1){
                return true;
            }
            else if(abs(y2-y1)==2 && y1 == 6){
                return true;
            }else{
                return false;
            }
        }
        return false;
    }
    return false;
}
bool Knight(int x1, int y1, int x2, int y2, Board *CurrBoard){
    if((abs(x2-x1) == 1 && abs(y2-y1) == 2 ) || (abs(y2-y1) == 1 && abs(x2-x1) == 2)){
        return true;
    }
    else {
        return false;
    }
}
bool Bishop(int x1, int y1, int x2, int y2, Board *CurrBoard){
    if(abs(x2-x1) == abs(y2-y1) && abs(x2-x1) != 0 && abs(y2-y1)!= 0 ){
        return true;
    }
    else{
        return false;
    }
}

bool Rook(int x1, int y1, int x2, int y2, Board *CurrBoard){
    if( ( x1==x2 && y2!=y1 ) || (x1!=x2 && y1==y2)){
        return true;
    }
    else{
        return false;
    }
}

bool Queen(int x1, int y1, int x2, int y2, Board *CurrBoard){
    if ( ( ( x1==x2 && y2!=y1 ) || (x1!=x2 && y1==y2))||
        ( abs(x2-x1) == abs(y2-y1) && abs(x2-x1) != 0 && abs(y2-y1)!= 0 )){
        return true;
    }
    else{
        return false;
    }
}


MoveList *ListAvailableMoves(Board *currBoard, bool activePlayer)
{  
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
   		if(j == 6)
                {
                    if(currBoard->game_board[i][j]->type == 1)
                    {
                        if(currBoard->game_board[i][7]->type == 0)
                        {
                                moveTemp = CreateMoveEntry();
                                moveTemp->Vector->Startpoint->x = i;
                                moveTemp->Vector->Startpoint->y = 6;
                                moveTemp->Vector->Endpoint->x = i;
                                moveTemp->Vector->Endpoint->y = 7;
                                moveTemp->Vector->Promotion = true;
                                moveTemp->Vector->AI = true;
                                AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(i+1 < 8 && currBoard->game_board[i+1][7]->type != 0 && currBoard->game_board[i+1][7]->color == 'b')
                        {
                                moveTemp = CreateMoveEntry();
                                moveTemp->Vector->Startpoint->x = i;
                                moveTemp->Vector->Startpoint->y = 6;
                                moveTemp->Vector->Endpoint->x = i+1;
                                moveTemp->Vector->Endpoint->y = 7;
                                moveTemp->Vector->Promotion = true;
                                moveTemp->Vector->AI = true;
                                AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(i-1 >= 0 && currBoard->game_board[i-1][7]->type != 0 && currBoard->game_board[i-1][7]->color == 'b')
                        {
                                moveTemp = CreateMoveEntry();
                                moveTemp->Vector->Startpoint->x = i;
                                moveTemp->Vector->Startpoint->y = 6;
                                moveTemp->Vector->Endpoint->x = i-1;
                                moveTemp->Vector->Endpoint->y = 7;
                                moveTemp->Vector->Promotion = true;
                                moveTemp->Vector->AI = true;
                                AppendMoveEntry(availableMoves, moveTemp);
                        }
                    }
                } 
                switch(currBoard->game_board[i][j]->type)
                {
                    case PAWN:
                        if(j+1 < 8 && currBoard->game_board[i][j+1]->type == 0)
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j + 1;
                
               
              
             
                                AppendMoveEntry(availableMoves, moveTemp);
            
                        }
                        if(i+1 < 8 && j+1 < 8 && currBoard->game_board[i + 1][j + 1]->type != 0 && currBoard->game_board[i + 1][j + 1]->color == 'b')//if the pawn can attack right
                        {
                            MoveEntry *moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i+1;
                            moveTemp->Vector->Endpoint->y = j+1;
           
         
        
       
                                AppendMoveEntry(availableMoves, moveTemp);
      
                        }
                        if(i-1 >= 0 && j+1< 8 && currBoard->game_board[i - 1][j + 1]->type != 0 && currBoard->game_board[i - 1][j + 1]->color == 'b') //if the pawn can attack left
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
                            if(j+2 < 8 && currBoard->game_board[i][j + 2]->type == 0 && currBoard->game_board[i][j + 1]->type == 0)
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
                        if(!(i+1 > 7 || j+2 > 7) && !(currBoard->game_board[i+1][j+2]->type != 0 && currBoard->game_board[i+1][j+2]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i+1;
                            moveTemp->Vector->Endpoint->y = j+2;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(!(i-1 < 0 || j+2 > 7) && !(currBoard->game_board[i-1][j+2]->type != 0 && currBoard->game_board[i-1][j+2]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i-1;
                            moveTemp->Vector->Endpoint->y = j+2;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(!(i+1 > 7 || j-2 < 0) && !(currBoard->game_board[i+1][j-2]->type != 0 && currBoard->game_board[i+1][j-2]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i+1;
                            moveTemp->Vector->Endpoint->y = j-2;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(!(i-1 < 0 || j-2 < 0) && !(currBoard->game_board[i-1][j-2]->type != 0 && currBoard->game_board[i-1][j-2]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i-1;
                            moveTemp->Vector->Endpoint->y = j-2;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(!(i+2 > 7 || j+1 > 7) && !(currBoard->game_board[i+2][j+1]->type != 0 && currBoard->game_board[i+2][j+1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i+2;
                            moveTemp->Vector->Endpoint->y = j+1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(!(i+2 > 7 || j-1 < 0) && !(currBoard->game_board[i+2][j-1]->type != 0 && currBoard->game_board[i+2][j-1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i+2;
                            moveTemp->Vector->Endpoint->y = j-1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(!(i-2 < 0 || j+1 > 7) && !(currBoard->game_board[i-2][j+1]->type != 0 && currBoard->game_board[i-2][j+1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i-2;
                            moveTemp->Vector->Endpoint->y = j+1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(!(i-2 < 0 || j-1 < 0) && !(currBoard->game_board[i-2][j-1]->type != 0 && currBoard->game_board[i-2][j-1]->color == 'w'))
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
                            if(i+k-1 >= 0 && j+k-1 >= 0 && k>1 && currBoard->game_board[i+k-1][j+k-1]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j+k]->type != 0 && currBoard->game_board[i+k][j+k]->color != 'b')
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
                            if(i+k+1 < 8 && j+k+1 < 8 && k<-1 && currBoard->game_board[i+k+1][j+k+1]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j+k]->type != 0 && currBoard->game_board[i+k][j+k]->color != 'b')
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
                            if(i-k+1 < 8 && j+k-1 >= 0 && k>1 && currBoard->game_board[i-k+1][j+k-1]->type != 0)
                                break;
                            if(currBoard->game_board[i-k][j+k]->type != 0 && currBoard->game_board[i-k][j+k]->color != 'b')
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
                            if(i+k-1 >= 0 && j-k+1 < 8 && k>1 && currBoard->game_board[i+k-1][j-k+1]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j-k]->type != 0 && currBoard->game_board[i+k][j-k]->color != 'b')
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
                            if(i+k-1 >= 0 && k>1 && currBoard->game_board[i+k-1][j]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j]->type != 0 && currBoard->game_board[i+k][j]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        for(int k = 1; i - k >= 0; k++)
                        {
                            if(i-k+1 < 8 && k>1 && currBoard->game_board[i-k+1][j]->type != 0)
                                break;
                            if(currBoard->game_board[i-k][j]->type != 0 && currBoard->game_board[i-k][j]->color != 'b')
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
                            if(j+k-1 >= 0 && k>1 && currBoard->game_board[i][j+k-1]->type != 0)
                                break;
                            if(currBoard->game_board[i][j+k]->type != 0 && currBoard->game_board[i][j+k]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j + k;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        for(int k = 1; j - k >= 0; k++)
                        {
                            if(j-k+1 < 8 && k>1 && currBoard->game_board[i][j-k+1]->type != 0)
                                break;
                            if(currBoard->game_board[i][j-k]->type != 0 && currBoard->game_board[i][j-k]->color != 'b')
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
                                        if(currBoard->game_board[k][0]->type != 0)//if the pieces are !0
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
                                        if(currBoard->game_board[k][0]->type != 0)//if the piece is 0
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
                            if(i+k-1 >= 0 && j+k-1 >= 0 && k<-1 && currBoard->game_board[i+k-1][j+k-1]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j+k]->type != 0 && currBoard->game_board[i+k][j+k]->color != 'b')
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
                            if(i+k+1 < 8 && j+k+1 < 8 && k<-1 && currBoard->game_board[i+k+1][j+k+1]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j+k]->type != 0 && currBoard->game_board[i+k][j+k]->color != 'b')
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
                            if(i-k+1 < 8 && j+k-1 >= 0 && k>1 && currBoard->game_board[i-k+1][j+k-1]->type != 0)
                                break;
                            if(currBoard->game_board[i-k][j+k]->type != 0 && currBoard->game_board[i-k][j+k]->color != 'b')
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
                            if(i+k-1 >= 0 && j-k+1 < 8 && k>1 && currBoard->game_board[i+k-1][j-k+1]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j-k]->type != 0 && currBoard->game_board[i+k][j-k]->color != 'b')
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
                            if(j+k-1 >= 0 && k>1 && currBoard->game_board[i][j+k-1]->type != 0)
                                break;
                            if(currBoard->game_board[i][j+k]->type != 0 && currBoard->game_board[i][j+k]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j + k;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        
                        //BACKWARD
                        for(int k = -1; j + k >= 0; k--)
                        {
                            if(j+k+1 < 8 && k<-1 && currBoard->game_board[i][j+k]->type != 0)
                                break;
                            if(currBoard->game_board[i][j+k]->type != 0 && currBoard->game_board[i][j+k]->color != 'b')
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
                            if(i+k-1 >= 0 && k>1 && currBoard->game_board[i+k-1][j]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j]->type != 0 && currBoard->game_board[i+k][j]->color != 'b')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        
                        for(int k = -1; i + k >= 0; k--)
                        {
                            if(i+k+1 < 8 && k<-1 && currBoard->game_board[i+k+1][j]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j]->type != 0 && currBoard->game_board[i+k][j]->color != 'b')
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
                        if(i+1 < 8 && !(currBoard->game_board[i+1][j]->type != 0 && currBoard->game_board[i+1][j]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + 1;
                            moveTemp->Vector->Endpoint->y = j;
     
    
  
   
                                AppendMoveEntry(availableMoves, moveTemp);
 
                        }
                        
                        if(i-1 >= 0 && !(currBoard->game_board[i-1][j]->type != 0 && currBoard->game_board[i-1][j]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i - 1;
                            moveTemp->Vector->Endpoint->y = j;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        
                        if(j+1 < 8 && !(currBoard->game_board[i][j+1]->type != 0 && currBoard->game_board[i][j+1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j + 1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        
                        if(j-1 >= 0 && !(currBoard->game_board[i][j-1]->type != 0 && currBoard->game_board[i][j-1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j - 1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        
                        if(i+1 < 8 && j+1 < 8 && !(currBoard->game_board[i+1][j+1]->type != 0 && currBoard->game_board[i+1][j+1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + 1;
                            moveTemp->Vector->Endpoint->y = j + 1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(i+1 < 8 && j-1 >= 0 && !(currBoard->game_board[i+1][j-1]->type != 0 && currBoard->game_board[i+1][j-1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + 1;
                            moveTemp->Vector->Endpoint->y = j - 1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        
                        if(i-1 >= 0 && j+1 < 8 && !(currBoard->game_board[i-1][j+1]->type != 0 && currBoard->game_board[i-1][j+1]->color == 'w'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i - 1;
                            moveTemp->Vector->Endpoint->y = j + 1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        
                        if(i-1 >= 0 && j-1 >= 0 && !(currBoard->game_board[i-1][j-1]->type != 0 && currBoard->game_board[i-1][j-1]->color == 'w'))
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
	if(currBoard->game_board[i][j]->color == 'b' && !activePlayer)
	{
           
            //promotion
	 if(j == 1)
                {
                    if(currBoard->game_board[i][j]->type == 1)
                    {
                        if(currBoard->game_board[i][0]->type == 0)
                        {
                                moveTemp = CreateMoveEntry();
                                moveTemp->Vector->Startpoint->x = i;
                                moveTemp->Vector->Startpoint->y = 1;
                                moveTemp->Vector->Endpoint->x = i;
                                moveTemp->Vector->Endpoint->y = 0;
                                moveTemp->Vector->Promotion = true;
                                moveTemp->Vector->AI = true;
                                AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(i+1 < 8 && currBoard->game_board[i+1][0]->type != 0 && currBoard->game_board[i+1][0]->color == 'w')
                        {
                                moveTemp = CreateMoveEntry();
                                moveTemp->Vector->Startpoint->x = i;
                                moveTemp->Vector->Startpoint->y = 1;
                                moveTemp->Vector->Endpoint->x = i+1;
                                moveTemp->Vector->Endpoint->y = 0;
                                moveTemp->Vector->Promotion = true;
                                moveTemp->Vector->AI = true;
                                AppendMoveEntry(availableMoves, moveTemp);
                        }
                        if(i-1 >= 0 && currBoard->game_board[i-1][1]->type != 0 && currBoard->game_board[i-1][0]->color == 'w')
                        {
                                moveTemp = CreateMoveEntry();
                                moveTemp->Vector->Startpoint->x = i;
                                moveTemp->Vector->Startpoint->y = 1;
                                moveTemp->Vector->Endpoint->x = i-1;
                                moveTemp->Vector->Endpoint->y = 0;
                                moveTemp->Vector->Promotion = true;
                                moveTemp->Vector->AI = true;
                                AppendMoveEntry(availableMoves, moveTemp);
                        }
                    }
                }   






































                switch(currBoard->game_board[i][j]->type)
                {
                    case PAWN:
                        if(j-1 >= 0 && currBoard->game_board[i][j-1]->type == 0)
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j - 1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(i+1 < 8 && j-1 >= 0 && currBoard->game_board[i + 1][j - 1]->type != 0 && currBoard->game_board[i + 1][j - 1]->color == 'w')//if the pawn can attack right
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i+1;
                            moveTemp->Vector->Endpoint->y = j-1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(i-1 >= 0 && j-1>= 0 && currBoard->game_board[i - 1][j - 1]->type != 0 && currBoard->game_board[i - 1][j - 1]->color == 'w') //if the pawn can attack left
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
                            if(j-2 >= 0 && currBoard->game_board[i][j - 2]->type == 0 && currBoard->game_board[i][j - 1]->type == 0)
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
                        if(!(i+1 > 7 || j+2 > 7) && !(currBoard->game_board[i+1][j+2]->type != 0 && currBoard->game_board[i+1][j+2]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i+1;
                            moveTemp->Vector->Endpoint->y = j+2;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(!(i-1 < 0 || j+2 > 7) && !(currBoard->game_board[i-1][j+2]->type != 0 && currBoard->game_board[i-1][j+2]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i-1;
                            moveTemp->Vector->Endpoint->y = j+2;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(!(i+1 > 7 || j-2 < 0) && !(currBoard->game_board[i+1][j-2]->type != 0 && currBoard->game_board[i+1][j-2]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i+1;
                            moveTemp->Vector->Endpoint->y = j-2;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(!(i-1 < 0 || j-2 < 0) && !(currBoard->game_board[i-1][j-2]->type != 0 && currBoard->game_board[i-1][j-2]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i-1;
                            moveTemp->Vector->Endpoint->y = j-2;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(!(i+2 > 7 || j+1 > 7) && !(currBoard->game_board[i+2][j+1]->type != 0 && currBoard->game_board[i+2][j+1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i+2;
                            moveTemp->Vector->Endpoint->y = j+1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(!(i+2 > 7 || j-1 < 0) && !(currBoard->game_board[i+2][j-1]->type != 0 && currBoard->game_board[i+2][j-1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i+2;
                            moveTemp->Vector->Endpoint->y = j-1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(!(i-2 < 0 || j+1 > 7) && !(currBoard->game_board[i-2][j+1]->type != 0 && currBoard->game_board[i-2][j+1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i-2;
                            moveTemp->Vector->Endpoint->y = j+1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(!(i-2 < 0 || j-1 < 0) && !(currBoard->game_board[i-2][j-1]->type != 0 && currBoard->game_board[i-2][j-1]->color == 'b'))
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
                            if(i+k-1 >= 0 && j+k-1 >= 0 && k>1 && currBoard->game_board[i+k-1][j+k-1]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j+k]->type != 0 && currBoard->game_board[i+k][j+k]->color != 'w')
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
                            if(i+k+1 < 8 && j+k+1 < 8 && k<-1 && currBoard->game_board[i+k+1][j+k+1]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j+k]->type != 0 && currBoard->game_board[i+k][j+k]->color != 'w')
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
                            if(i-k+1 < 8 && j+k-1 >= 0 && k>1 && currBoard->game_board[i-k+1][j+k-1]->type != 0)
                                break;
                            if(currBoard->game_board[i-k][j+k]->type != 0 && currBoard->game_board[i-k][j+k]->color != 'w')
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
                            if(i+k-1 >= 0 && j-k+1 < 8 && k>1 && currBoard->game_board[i+k-1][j-k+1]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j-k]->type != 0 && currBoard->game_board[i+k][j-k]->color != 'w')
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
                            if(i+k-1 >= 0 && k>1 && currBoard->game_board[i+k-1][j]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j]->type != 0 && currBoard->game_board[i+k][j]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j;
                       
                      
                     
                    
                                AppendMoveEntry(availableMoves, moveTemp);
                   
                        }
                        for(int k = 1; i - k >= 0; k++)
                        {
                            if(i-k+1 < 8 && k>1 && currBoard->game_board[i-k+1][j]->type != 0)
                                break;
                            if(currBoard->game_board[i-k][j]->type != 0 && currBoard->game_board[i-k][j]->color != 'w')
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
                            if(j+k-1 >= 0 && k>1 && currBoard->game_board[i][j+k-1]->type != 0)
                                break;
                            if(currBoard->game_board[i][j+k]->type != 0 && currBoard->game_board[i][j+k]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j + k;
             
            
           
          
                                AppendMoveEntry(availableMoves, moveTemp);
         
                        }
                        for(int k = 1; j - k >= 0; k++)
                        {
                            if(j-k+1 < 8 && k>1 && currBoard->game_board[i][j-k+1]->type != 0)
                                break;
                            if(currBoard->game_board[i][j-k]->type != 0 && currBoard->game_board[i][j-k]->color != 'w')
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
                                        if(currBoard->game_board[k][7]->type != 0)//if the pieces are !0
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
                                        if(currBoard->game_board[k][7]->type != 0)//if the piece is 0
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
                            if(i+k-1 >= 0 && j+k-1 >= 0 && k>1 && currBoard->game_board[i+k-1][j+k-1]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j+k]->type != 0 && currBoard->game_board[i+k][j+k]->color != 'w')
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
                            if(i+k+1 < 8 && j+k+1 < 8 && k<-1 && currBoard->game_board[i+k+1][j+k+1]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j+k]->type != 0 && currBoard->game_board[i+k][j+k]->color != 'w')
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
                            if(i-k+1 < 8 && j+k-1 >= 0 && k>1 && currBoard->game_board[i-k+1][j+k-1]->type != 0)
                                break;
                            if(currBoard->game_board[i-k][j+k]->type != 0 && currBoard->game_board[i-k][j+k]->color != 'w')
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
                            if(i+k-1 >= 0 && j-k+1 < 8 && k>1 && currBoard->game_board[i+k-1][j-k+1]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j-k]->type != 0 && currBoard->game_board[i+k][j-k]->color != 'w')
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
                            if(j+k-1 >= 0 && k>1 && currBoard->game_board[i][j+k-1]->type != 0)
                                break;
                            if(currBoard->game_board[i][j+k]->type != 0 && currBoard->game_board[i][j+k]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j + k;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        
                        //BACKWARD
                        for(int k = -1; j + k >= 0; k--)
                        {
                            if(j+k+1 < 8 && k<-1 && currBoard->game_board[i][j+k]->type != 0)
                                break;
                            if(currBoard->game_board[i][j+k]->type != 0 && currBoard->game_board[i][j+k]->color != 'w')
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
                            if(i+k-1 >= 0 && k>1 && currBoard->game_board[i+k-1][j]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j]->type != 0 && currBoard->game_board[i+k][j]->color != 'w')
                                break;
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + k;
                            moveTemp->Vector->Endpoint->y = j;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        
                        for(int k = -1; i + k >= 0; k--)
                        {
                            if(i+k+1 < 8 && k<-1 && currBoard->game_board[i+k+1][j]->type != 0)
                                break;
                            if(currBoard->game_board[i+k][j]->type != 0 && currBoard->game_board[i+k][j]->color != 'w')
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
                        if(i+1 < 8 && !(currBoard->game_board[i+1][j]->type != 0 && currBoard->game_board[i+1][j]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + 1;
                            moveTemp->Vector->Endpoint->y = j;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        
                        if(i-1 >= 0 && !(currBoard->game_board[i-1][j]->type != 0 && currBoard->game_board[i-1][j]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i - 1;
                            moveTemp->Vector->Endpoint->y = j;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        
                        if(j+1 < 8 && !(currBoard->game_board[i][j+1]->type != 0 && currBoard->game_board[i][j+1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j + 1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        
                        if(j-1 >= 0 && !(currBoard->game_board[i][j-1]->type != 0 && currBoard->game_board[i][j-1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i;
                            moveTemp->Vector->Endpoint->y = j - 1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        
                        if(i+1 < 8 && j+1 < 8 && !(currBoard->game_board[i+1][j+1]->type != 0 && currBoard->game_board[i+1][j+1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + 1;
                            moveTemp->Vector->Endpoint->y = j + 1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        if(i+1 < 8 && j-1 >= 0 && !(currBoard->game_board[i+1][j-1]->type != 0 && currBoard->game_board[i+1][j-1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i + 1;
                            moveTemp->Vector->Endpoint->y = j - 1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        
                        if(i-1 >= 0 && j+1 < 8 && !(currBoard->game_board[i-1][j+1]->type != 0 && currBoard->game_board[i-1][j+1]->color == 'b'))
                        {
                            moveTemp = CreateMoveEntry();
                            moveTemp->Vector->Startpoint->x = i;
                            moveTemp->Vector->Startpoint->y = j;
                            moveTemp->Vector->Endpoint->x = i - 1;
                            moveTemp->Vector->Endpoint->y = j + 1;




                                AppendMoveEntry(availableMoves, moveTemp);

                        }
                        
                        if(i-1 >= 0 && j-1 >= 0 && !(currBoard->game_board[i-1][j-1]->type != 0 && currBoard->game_board[i-1][j-1]->color == 'b'))
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

void UpdateBoard(Board *currBoard, MoveEntry *currMove)
{
    if(currMove == NULL)
	return;
   int x1 = currMove->Vector->Startpoint->x;
   int y1 = currMove->Vector->Startpoint->y;
   int x2 = currMove->Vector->Endpoint->x;
   int y2 = currMove->Vector->Endpoint->y; 
   char pColor = 0;
   char pChoice = 0;
    if(currMove->Vector->Promotion)
    {
	if(currMove->Vector->AI)
	{
	    currBoard->game_board[x1][y1]->type = 0; 
	    pColor = currBoard->game_board[x1][y1]->color;
	    currBoard->game_board[x1][y1]->color = 'e';
	    currBoard->game_board[x1][y1]->val = 0;
	    currBoard->game_board[x2][y2]->type = queen;
	    currBoard->game_board[x2][y2]->color = pColor;
	    currBoard->game_board[x2][y2]->val = 90*(pColor == 'w' ? 1 : -1);
	    return;
	}
    }
    if((y2 == 0 || y2 == 7) && (currBoard->game_board[x1][y1]->type == pawn) && (!currMove->Vector->AI))
    {
	printf("Your Pawn is eligible for promotion. Promote to (Q)ueen, (K)night, (R)ook, or (B)ishop?\n");	
    	scanf("%c", &pChoice);
	switch(pChoice)
	{
		case 'Q':
		case 'q':
			currBoard->game_board[x1][y1]->type = 0;
			currBoard->game_board[x1][y1]->color = 'e';
			currBoard->game_board[x1][y1]->val = 0;
			currBoard->game_board[x2][y2]->type = queen;
			currBoard->game_board[x2][y2]->color = (y2 == 0 ? 'b' : 'w');
			currBoard->game_board[x2][y2]->val = 90*(y2 == 0 ? -1 : 1);
			break;
		case 'K':
                case 'k':
                        currBoard->game_board[x1][y1]->type = 0;
                        currBoard->game_board[x1][y1]->color = 'e';
                        currBoard->game_board[x1][y1]->val = 0;
                        currBoard->game_board[x2][y2]->type = knight;
                        currBoard->game_board[x2][y2]->color = (y2 == 0 ? 'b' : 'w');
                        currBoard->game_board[x2][y2]->val = 30*(y2 == 0 ? -1 : 1);
                        break;
		case 'R':
                case 'r':
                        currBoard->game_board[x1][y1]->type = 0;
                        currBoard->game_board[x1][y1]->color = 'e';
                        currBoard->game_board[x1][y1]->val = 0;
                        currBoard->game_board[x2][y2]->type = rook;
                        currBoard->game_board[x2][y2]->color = (y2 == 0 ? 'b' : 'w');
                        currBoard->game_board[x2][y2]->val = 50*(y2 == 0 ? -1 : 1);
                        break;
		case 'B':
                case 'b':
                        currBoard->game_board[x1][y1]->type = 0;
                        currBoard->game_board[x1][y1]->color = 'e';
                        currBoard->game_board[x1][y1]->val = 0;
                        currBoard->game_board[x2][y2]->type = bishop;
                        currBoard->game_board[x2][y2]->color = (y2 == 0 ? 'b' : 'w');
                        currBoard->game_board[x2][y2]->val = 30*(y2 == 0 ? -1 : 1);
                        break;
		default:
                        currBoard->game_board[x1][y1]->type = 0;
                        currBoard->game_board[x1][y1]->color = 'e';
                        currBoard->game_board[x1][y1]->val = 0;
                        currBoard->game_board[x2][y2]->type = queen;
                        currBoard->game_board[x2][y2]->color = (y2 == 0 ? 'b' : 'w');
                        currBoard->game_board[x2][y2]->val = 90*(y2 == 0 ? -1 : 1);
                        break;
	}
    }

    Piece *currPiece = GetPiece(x1, y1, currBoard);
    Piece *endPiece = GetPiece(x2, y2, currBoard); 
    Piece *specialPiece = NULL;
    short int type = GetPieceType(currPiece);
    short int val = GetPieceValue(currPiece);
    char color = GetPieceColor(currPiece);
    bool longMove = currPiece->longMove;
    bool epSwitch1 = currPiece->epSwitch1;
    bool epSwitch = currPiece->epSwitch;
    char castle = 0;
    if(currPiece->type == 1 && x2-x1 != 0 && endPiece->type == 0)    
    {
	currBoard->game_board[x2][y1]->type = 0;
	currBoard->game_board[x2][y1]->val = 0;
	currBoard->game_board[x2][y1]->color = 'e';
	currPiece->type = 0;
	currPiece->val = 0;
	currPiece->color = 'e';
	endPiece->type = type;
	endPiece->val = val;
	endPiece->color = color;
	endPiece->longMove = longMove;
	endPiece->epSwitch1 = epSwitch1;
	endPiece->epSwitch = epSwitch;
	return;
    }

    if(currMove->Vector->Castling)
    {
	if(!currMove->Vector->AI)
	{
		printf("Would you like to perform a castle?(Y/N)\t");
		scanf(" %c", &castle);
		if(castle == 'y' || castle == 'Y')
		{
			if(x1 == 0)
			{
				endPiece->type = type;
				endPiece->val = val;
				endPiece->hasMoved = true;
				endPiece->color = color;
				endPiece->longMove = longMove;	
				endPiece->epSwitch1 = epSwitch1;
				endPiece->epSwitch = epSwitch;
				specialPiece = currBoard->game_board[2][y1];
				specialPiece->type = king;
				specialPiece->val = 900 * y1 == 0 ? 1 : -1;
				specialPiece->hasMoved = true;
				specialPiece->color = y1 == 0 ? 'w' : 'b';
				specialPiece->longMove = false;
				specialPiece->epSwitch1 = false;
				specialPiece->epSwitch = false;
				currPiece->type = 0;
				currPiece->val = 0;
				currPiece->color = 'e';
				currBoard->game_board[4][y2]->type = 0;
				currBoard->game_board[4][y2]->color = 'e';
				currBoard->game_board[4][y2]->val = 0;
			}
			if(x1 == 7)
			{ 
                                endPiece->type = type;
                                endPiece->val = val;
                                endPiece->hasMoved = true;
                                endPiece->color = color;
                                endPiece->longMove = longMove;  
                                endPiece->epSwitch1 = epSwitch1;
                                endPiece->epSwitch = epSwitch;
                                specialPiece = currBoard->game_board[6][y1];
                                specialPiece->type = king;
                                specialPiece->val = 900 * y1 == 0 ? 1 : -1;
                                specialPiece->hasMoved = true;
                                specialPiece->color = y1 == 0 ? 'w' : 'b';
                                specialPiece->longMove = false;
                                specialPiece->epSwitch1 = false;
                                specialPiece->epSwitch = false;
                                currPiece->type = 0;
                                currPiece->val = 0;
                                currPiece->color = 'e';
                        	currBoard->game_board[4][y2]->type = 0;
                                currBoard->game_board[4][y2]->color = 'e';
                                currBoard->game_board[4][y2]->val = 0;
			}
		}
	}
	else
	{
                        if(x1 == 0)
                        {
                                endPiece->type = type;
                                endPiece->val = val;
                                endPiece->hasMoved = true;
                                endPiece->color = color;
                                endPiece->longMove = longMove;
                                endPiece->epSwitch1 = epSwitch1;
                                endPiece->epSwitch = epSwitch;
                                specialPiece = currBoard->game_board[2][y1];
                                specialPiece->type = king;
                                specialPiece->val = 900 * y1 == 0 ? 1 : -1;
                                specialPiece->hasMoved = true;
                                specialPiece->color = y1 == 0 ? 'w' : 'b';
                                specialPiece->longMove = false;
                                specialPiece->epSwitch1 = false;
                                specialPiece->epSwitch = false;
                                currPiece->type = 0;
                                currPiece->val = 0;
                                currPiece->color = 'e';
                        	currBoard->game_board[4][y2]->type = 0;
                                currBoard->game_board[4][y2]->color = 'e';
                                currBoard->game_board[4][y2]->val = 0;
			}
                        if(x1 == 7)
                        { 
                                endPiece->type = type;
                                endPiece->val = val;
                                endPiece->hasMoved = true;
                                endPiece->color = color;
                                endPiece->longMove = longMove;  
                                endPiece->epSwitch1 = epSwitch1;
                                endPiece->epSwitch = epSwitch;
                                specialPiece = currBoard->game_board[6][y1];
                                specialPiece->type = king;
                                specialPiece->val = 900 * y1 == 0 ? 1 : -1;
                                specialPiece->hasMoved = true;
                                specialPiece->color = y1 == 0 ? 'w' : 'b';
                                specialPiece->longMove = false;
                                specialPiece->epSwitch1 = false;
                                specialPiece->epSwitch = false;
                                currPiece->type = 0;
                                currPiece->val = 0;
                                currPiece->color = 'e';
                        	currBoard->game_board[4][y2]->type = 0;
                                currBoard->game_board[4][y2]->color = 'e';
                                currBoard->game_board[4][y2]->val = 0;
			}
	}
    } 

    endPiece->type = type;
    endPiece->val = val;
    endPiece->color = color;
    endPiece->hasMoved = true;
    endPiece->longMove = longMove;  
    endPiece->epSwitch1 = epSwitch1;
    endPiece->epSwitch = epSwitch;

    if(!currMove->Vector->Castling)
    {
        currPiece->type = EMPTY;
        currPiece->val = 0; 
    	currPiece->color = 'n';
    }
    if(endPiece->type == 1 && ((y1 == 1 && y2 == 3)||(y1 == 6 && y2 == 4)))
    {
	endPiece->epSwitch1 = true;
	endPiece->epSwitch = true;
    }

    for(int k = 0; k < 8; k++)
    {
	if(currBoard->game_board[k][3]->epSwitch1)
		currBoard->game_board[k][3]->epSwitch1 = false;
	else 
		currBoard->game_board[k][3]->epSwitch = false;
    }
}



bool Block(int x1, int y1, int x2, int y2, Board *CurrBoard){
    if (GetPieceColor(GetPiece(x1,y1,CurrBoard)) == GetPieceColor(GetPiece(x2,y2,CurrBoard))){
        return true;
    }
    int count;
    
    //bishop
	    

     if (GetPieceType(GetPiece(x1,y1,CurrBoard)) == 3){
	count = 1;
        if( (x2>x1) && (y2>y1)){
            for(int i=1; i < x2-x1; i++){
                if (GetPieceType(GetPiece(x1+i, y1+i, CurrBoard)) != 0){
                    count = count +1;
                }
            }
        }
        
        if ((x2>x1) && (y2<y1)) {
            for(int i=1; i<x2-x1; i++){
                if (GetPieceType(GetPiece(x1+i, y1-i, CurrBoard)) != 0){
                    count = count+1;
                }
            }
        }
        
        if ((x2<x1) && (y2>y1)) {
            for (int i=1; i<y2-y1; i++){
                if (GetPieceType(GetPiece(x1-i, y1+i, CurrBoard)) != 0){
                    count =count+1;
                }
            }
        }
        
        if ((x2<x1) && (y2<y1)) {
            for (int i=1; i<y1-y2; i++){
                if (GetPieceType(GetPiece(x1-i, y1-i, CurrBoard)) != 0){
                    count = count+1;
                }
            }
        }
        
        if (count>1) {
            return true;
        }
    }
    //rook
    if (GetPieceType(GetPiece(x1,y1,CurrBoard)) == 4 ){
        count = 1;
        if(x1==x2){
            for(int i=1; i<abs(y2-y1); i++){
                if(y2>y1){
                    if (GetPieceType(GetPiece(x1,y1+i,CurrBoard)) != 0){
                        count = count+1;
                    }
                }
                if(y2<y1){
                    if (GetPieceType(GetPiece(x1,y1-i,CurrBoard)) != 0){
                        count = count+1;
                    }
                }
            }
            if (count>1) {
                return true;
            }
        }
        if(y1==y2){
            for(int i=1; i<abs(x2-x1); i++){
                if(x2>x1){
                    if (GetPieceType(GetPiece(x1+i,y1,CurrBoard)) != 0){
                        count = count+1;
                    }
                }
                if(x2<x1){
                    if (GetPieceType(GetPiece(x1-i,y1,CurrBoard)) != 0){
                        count = count+1;
                    }
                }
            }
            if (count>1) {
                return true;
            }
        }
    }
    //queen
    if (GetPieceType(GetPiece(x1,y1,CurrBoard)) == 6){
        
        if(x1 != x2 && y1 != y2){
            count = 1;
            if( (x2>x1) && (y2>y1)){
                for(int i=1; i < x2-x1; i++){
                    if (GetPieceType(GetPiece(x1+i, y1+i, CurrBoard)) != 0){
                        count = count +1;
                    }
                }
            }
            
            if ((x2>x1) && (y2<y1)) {
                for(int i=1; i<x2-x1; i++){
                    if (GetPieceType(GetPiece(x1+i, y1-i, CurrBoard)) != 0){
                        count = count+1;
                    }
                }
            }
            
            if ((x2<x1) && (y2>y1)) {
                for (int i=1; i<y2-y1; i++){
                    if (GetPieceType(GetPiece(x1-i, y1+i, CurrBoard)) != 0){
                        count =count+1;
                    }
                }
            }
            
            if ((x2<x1) && (y2<y1)) {
                for (int i=1; i<y1-y2; i++){
                    if (GetPieceType(GetPiece(x1-i, y1-i, CurrBoard)) != 0){
                        count = count+1;
                    }
                }
            }
            
            if (count>1) {
                return true;
            }
        }
        
        else if(x1==x2 || y1==y2){
            count = 1;
            if(x1==x2){
                for(int i=1; i<abs(y2-y1); i++){
                    if(y2>y1){
                        if (GetPieceType(GetPiece(x1,y1+i,CurrBoard)) != 0){
                            count = count+1;
                        }
                    }
                    if(y2<y1){
                        if (GetPieceType(GetPiece(x1,y1-i,CurrBoard)) != 0){
                            count = count+1;
                        }
                    }
                }
                if (count>1) {
                    return true;
                }
            }
            if(y1==y2){
                for(int i=1; i<abs(x2-x1); i++){
                    if(x2>x1){
                        if (GetPieceType(GetPiece(x1+i,y1,CurrBoard)) != 0){
                            count = count+1;
                        }
                    }
                    if(x2<x1){
                        if (GetPieceType(GetPiece(x1-i,y1,CurrBoard)) != 0){
                            count = count+1;
                        }
                    }
                }
                if (count>1) {
                    return true;
                }
            }
        }
    }
    //pawn
    if(GetPieceType(GetPiece(x1,y1,CurrBoard)) ==1){
        if(x2==x1){
          if(GetPieceType(GetPiece(x2,y2,CurrBoard)) !=0){
             return true;
             }
         }
        if( y1 == 2 || y1 == 7){
            if(abs(y2-y1) == 1){
                if(y1 == 1){
                    if(GetPieceType(GetPiece(x1,y1+1,CurrBoard)) != 0){
                        return true;
                    }
                }
                if(y1 == 7){
                    if(GetPieceType(GetPiece(x1,y1-1,CurrBoard)) != 0){
                        return true;
                    }
                }
            }
            if(abs(y2-y1) == 2){
                if(y1 == 1){
                    if(GetPieceType(GetPiece(x1,y1+1,CurrBoard)) != 0 && GetPieceType(GetPiece(x1,y1+2,CurrBoard)) != 0){
                        return true;
                    }
                }
                if(y1 == 7){
                    if(GetPieceType(GetPiece(x1,y1-1,CurrBoard)) != 0 && GetPieceType(GetPiece(x1,y1-2,CurrBoard)) != 0){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void AppendMoveEntry(MoveList *list, MoveEntry *e)
{
	if(list->Last)//list is not empty
	{
		list->Last->Next = e;
		e->Prev = list->Last;
		e->Next = NULL;
		list->Last = e;	
	}//list is empty
	else
	{
		list->Last = e;
		list->First = e;
		e->Next = NULL;
		e->Prev = NULL;
	}

	list->ListLength++;
}

bool Promotion(int x1, int y1, int x2, int y2, Board *currBoard){
    if(GetPieceType(currBoard->game_board[x1][y1]) == 1 && (y2 == 0 || y2 == 7)){
        return true;
    }
    return false;
}

bool EnPassant(int x1,int y1,int x2,int y2, Board* currBoard){
    char activePlayer = currBoard->game_board[x1][y1]->color;
    if(currBoard->game_board[x1][y1]->type == 1){// pawn move
        if((y2-y1) == 1 && abs(x2-x1) == 1){//white
            if(currBoard->game_board[x2][y1]->type == 1 && currBoard->game_board[x2][y1]->color != activePlayer && currBoard->game_board[x2][y1]->epSwitch == 1){
                return true;
            }
        }
        if((y1-y2) == 1 && abs(x2-x1) == 1){//black
            if(currBoard->game_board[x2][y1]->type == 1 && currBoard->game_board[x2][y1]->color != activePlayer && currBoard->game_board[x2][y1]->epSwitch == 1){
                return true;
            }
        }
        return false;
    }
    return false;
}

bool Check(Board* currBoard, bool activePlayer){
    MoveList *check = ListAvailableMoves(currBoard, !activePlayer);
    MoveEntry *e = check->First;
    MoveEntry *n;
    if(e->Next)
	n = e->Next;
    else
        return true;
    while(check){
        if(e == NULL)
		break;
	if(GetPieceType(currBoard->game_board[e->Vector->Endpoint->x][e->Vector->Endpoint->y]) == 5 ){
            	DeleteMoveList(check);
		return true;
        }
        e=n;
	if(e != NULL)
	    n = e->Next;
    }
    DeleteMoveList(check);
    return false;
}

bool RepetitiveMoveThree(MoveList* list){
    if(list->ListLength < 7)
	return false;
    int x1 = list->Last->Vector->Startpoint->x;
    int y1 = list->Last->Vector->Startpoint->y;
    int x2 = list->Last->Vector->Endpoint->x;
    int y2 = list->Last->Vector->Endpoint->y;
    int count = 0;
    if(list->Last->Prev->Prev->Vector->Startpoint->x == x2 && list->Last->Prev->Prev->Vector->Startpoint->y == y2 && list->Last->Prev->Prev->Vector->Endpoint->x == x1 && list->Last->Prev->Prev->Vector->Endpoint->y == y1){
        count++;
    }
    if(list->Last->Prev->Prev->Prev->Prev->Vector->Startpoint->x == x1 && list->Last->Prev->Prev->Prev->Prev->Vector->Startpoint->y == y1 && list->Last->Prev->Prev->Prev->Prev->Vector->Endpoint->x == x2 && list->Last->Prev->Prev->Prev->Prev->Vector->Endpoint->y == y2){
        count++;
    }
    if(list->Last->Prev->Prev->Prev->Prev->Prev->Prev->Vector->Startpoint->x == x2 && list->Last->Prev->Prev->Prev->Prev->Prev->Prev->Vector->Startpoint->y == y2 && list->Last->Prev->Prev->Prev->Prev->Prev->Prev->Vector->Endpoint->x == x1 && list->Last->Prev->Prev->Prev->Prev->Prev->Prev->Vector->Endpoint->y == y1){
        count++;
    }
    if (count == 3){
        return true;
    }
    else{
        return false;
    }
}

bool Castling(int x1,int y1,int x2,int y2, Board *currBoard) {
    if(GetPieceType(currBoard->game_board[x1][y1]) == 4 && currBoard->game_board[x1][y1]->hasMoved == 0){
        if(IsValidMove(x1, y1, x2, y2, currBoard) == 1 && y1 == y2 ){
           if(((x1-1>=0) && !currBoard->game_board[x1-1][y1]->hasMoved) || ((x1+1<8) && !currBoard->game_board[x1+1][y1]->hasMoved)){ 
                return true;
            }
        }
    }
    return false;
}

bool CheckMate(Board * currBoard, bool activePlayer)
{
    MoveList *availableMoves = ListAvailableMoves(currBoard, activePlayer);
    MoveEntry *currMove = availableMoves->First;
    Board *testBoard = CreateBoard();
    CopyBoard(currBoard, testBoard);
    for(int i = 0; i < availableMoves->ListLength; i++)
    {
        if(!currMove)
	    break;
	UpdateBoard(testBoard, currMove);
	if(!Check(testBoard, activePlayer))
        {
            DeleteBoard(testBoard);
            DeleteMoveList(availableMoves);
            return false;
        }
        if(currMove)
    	    currMove = currMove->Next;
    	CopyBoard(currBoard, testBoard);
    }
    DeleteBoard(testBoard);
    DeleteMoveList(availableMoves);
    return true;
}














