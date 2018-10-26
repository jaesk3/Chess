#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "Move.h"
#include "Chess.h"
#include "Opponent.h"
#include <assert.h>
#define ASCII_RED "\x1b[31m"
#define ASCII_GREEN "\x1b[32m"
#define ASCII_BLUE "\x1b[34m"
#define ASCII_RESET "\x1b[0m"


void DisplayBoard(Board *GameBoard);
void PrintHorizontalBorder();
void SaveFile(MoveList *MoveList, char *fname);
Board *LoadFile(MoveList *MoveList, char *fname);
int WhoWon(Board *GameBoard);
void DisplayGUI(Board *GameBoard);

int main() {
	Board *CCBoard = NULL;
	BoardList *AllBoards = CreateBoardList();
	Board *MasterBoard = NULL;
	MoveList *MoveList = CreateMoveList();
	char colorInput, winner[7];
	bool activePlayer = true; // If 1, then Player1's turn. If 0, then Player2's turn
	bool realPlayer = false;
	char input[5];
	char fname[20];
	int x1, y1, x2, y2;
	int pMode = 0;
	char loadFile, saveInput;
	FILE *file;
	char difficulty;
	int searchLevel = 4;
	MoveEntry *HintMove = NULL;
	bool uFlag = false;
	bool eFlag = false;
	int eCount = 0;

	printf("-----Welcome to Chess Please!-----\n");
	printf("Do you want to load previous game? (y/n) : ");
	scanf("%c", &loadFile);

	if (loadFile == 'y') {
		file = fopen("log.txt", "a");//append to previously written file
		while (MasterBoard == NULL) {
			printf("Enter file name to load (example.txt) : ");
			scanf("%s", fname);
			for (int i = 0; fname[i] != 0 && fname[i] != '\n'; i++) {
				if (fname[i] == '.') {
                 	eFlag = true;
					break;
                }
				eCount = i;
            }
			if (!eFlag) {
				fname[eCount+1] = '.';
				fname[eCount+2] = 't';
				fname[eCount+3] = 'x';
				fname[eCount+4] = 't';
				fname[eCount+5] = 0;
			}
			MasterBoard = LoadFile(MoveList, fname);
			SetPlayer('W', false, MasterBoard->Player1);
			SetPlayer('B', true, MasterBoard->Player2);
			realPlayer = true;
			pMode = 1;
			activePlayer = MasterBoard->currPlayer;
		}
	}
	else {
		file = fopen("log.txt", "w+");

		printf("Please choose your color : \n");
		printf("\tW = White\tB = Black\t");
		scanf(" %c", &colorInput);
	
		MasterBoard = CreateBoard();
		if (colorInput == 'W' || colorInput == 'w') {
			SetPlayer('W', false, MasterBoard->Player1);
			SetPlayer('B', true, MasterBoard->Player2);
			realPlayer = true;
		}
		else if(colorInput == 'B' || colorInput == 'b') {
			SetPlayer('W', true, MasterBoard->Player1);
			SetPlayer('B', false, MasterBoard->Player2);
		}
			
		printf("Play in (2) player mode or (1) player mode?\t");
		scanf("%d", &pMode);
		
		if(pMode == 1)
		{
			printf("Difficulty (E)asy, (M)edium, or (H)ard?\n");		
			scanf(" %c", &difficulty);	
			switch(difficulty)
			{
				case 'E':
				case 'e':
					searchLevel = 1;
					break;
				case 'M':
				case 'm':
					searchLevel = 3;
					break;
				case 'H':
				case 'h':
					searchLevel = 4;
					break;
				default:
					break;
			}	
		}	
		InitializeBoard(MasterBoard);
	}
	
	// Game starts
	while (!isGameOver(MasterBoard) && !(CheckMate(MasterBoard, true)) && !(CheckMate(MasterBoard, false))) {

		DisplayBoard(MasterBoard);	
		// If the user inputs 'w', he/she goes first
		if (activePlayer == realPlayer) {
			printf("--Type \"save\" to save current game--\n");
			printf("--Type \"quit\" to quit current game--\n");
			printf("--Type \"undo\" to undo current game--\n");
			printf("--Type \"hint\" to get a hint--\n");
			printf("Enter your move (ex \"e2e4\") : ");
            scanf("%s", input);
            if (strcmp(input, "save") == 0) {
				printf("Enter the file name to save : ");
				scanf("%s", fname);
				SaveFile(MoveList, fname);	
				continue;
			}
			else if (strcmp(input, "quit") == 0) {
				printf("Do you want to save current game? (y/n) : ");
				scanf(" %c", &saveInput);
				if (saveInput == 'y') {
					printf("Enter the file name to save : ");
					scanf("%s", fname);
					SaveFile(MoveList, fname);
				}
				printf("--Program exited--\n");
				DeleteMoveList(MoveList);
			    MoveList = NULL;
			    DeleteBoard(MasterBoard);
			    DeleteBoardList(AllBoards);
			    MasterBoard = NULL;
				fclose(file);
				return 0;
			}
			else if (strcmp(input, "undo") == 0) {	
				if (AllBoards->Last == NULL) {
					continue;
				}
				if (AllBoards->Last->Prev == NULL) {
					printf("Nothing to undo\n");
					continue;
				}
				
				DeleteLastBoard(AllBoards);
				DeleteLastEntry(MoveList);
				DeleteLastEntry(MoveList);
				if (uFlag) {	
					DeleteLastBoard(AllBoards);
				}
				CopyBoard(AllBoards->Last, MasterBoard);
				DisplayBoard(MasterBoard);
				printf("Undo successful!\n");
				uFlag = true;
				continue;
			}
			else if (strcmp(input, "hint") == 0) {
				HintMove = FindBestMove(MasterBoard, activePlayer, 3);
				printf("Move ");
				printf("%c", HintMove->Vector->Startpoint->x + 97);
				printf("%d", HintMove->Vector->Startpoint->y + 1);
				printf("%c", HintMove->Vector->Endpoint->x + 97);
				printf("%d", HintMove->Vector->Endpoint->y + 1);
				printf(".\n");
				DeleteMoveEntry(HintMove);			
				continue;
			}
			else {
				input[0] = tolower(input[0]) - 97;
				input[2] = tolower(input[2]) - 97;

				x1 = input[0];
				y1 = input[1] - 49;
				x2 = input[2];
				y2 = input[3] - 49;
				
				if (x1<0 || x1>7 || y1<0 || y1>7 || x2<0 || x2>7 || y2<0 || y1>7) {
					printf("Invalid Move!\n");
					continue;
				}
		
				if ((MasterBoard->game_board[x1][y1]->color == 'w' ? true : false) != activePlayer) {
					printf("Invalid Move!\n");
					continue;
				}

				MoveVector *MoveVector1 = NewMoveVector(MasterBoard, x1, y1, x2, y2);

				if (MoveVector1 == NULL) {
					printf("Invalid move!\n");
					continue;
				}
				else {
					AppendMove(MoveList, MoveVector1);	
					CCBoard = CreateBoard();
					CopyBoard(MasterBoard, CCBoard);
					AppendBoard(AllBoards, CCBoard);
					UpdateBoard(MasterBoard, MoveList->Last);
					input[0] = x1 + 97;
					input[1] = y1 + 49;
					input[2] = x2 + 97;
					input[3] = y2 + 49;
					fprintf(file, "%lu. %s  ", MoveList->ListLength + 1, input);
				}
			}	
		}
		else {
			// If the mode is p vs AI
			if(pMode == 1)
			{
				MoveEntry *MoveEntry1 = FindBestMove(MasterBoard, activePlayer, searchLevel);
				CCBoard = CreateBoard();
				CopyBoard(MasterBoard, CCBoard);
				AppendBoard(AllBoards, CCBoard);
				UpdateBoard(MasterBoard, MoveEntry1);
				AppendMoveEntry(MoveList, MoveEntry1);
				input[0] = MoveEntry1->Vector->Startpoint->x + 97;
				input[1] = MoveEntry1->Vector->Startpoint->y + 49;
				input[2] = MoveEntry1->Vector->Endpoint->x + 97;
				input[3] = MoveEntry1->Vector->Endpoint->y + 49;
				fprintf(file, "%lu. %s  ", MoveList->ListLength + 1, input);
			}
			// If the mode is p vs p 
			else
			{
				printf("--Type \"save\" to save current game--\n");
				printf("--Type \"quit\" to quit current game--\n");
				printf("--Type \"undo\" to undo current game--\n");
				printf("--Type \"hint\" to get a hint--\n");
				printf("Enter your move (ex \"e2e4\") : ");
                scanf("%s", input);
                if (strcmp(input, "save") == 0) {
					printf("Enter the file name to save : ");
					scanf("%s", fname);
					SaveFile(MoveList, fname);
					continue;
				}
				else if (strcmp(input, "quit") == 0) {
					printf("Do you want to save current game? (y/n) : ");
					scanf(" %c", &saveInput);
					if (saveInput == 'y') {
						printf("Enter the file name to save : ");
						scanf("%s", fname);
						SaveFile(MoveList, fname);
					}
					printf("--Program exited--\n");
					DeleteMoveList(MoveList);
				    MoveList = NULL;
				    DeleteBoard(MasterBoard);
			        DeleteBoardList(AllBoards);
			        MasterBoard = NULL;				
					fclose(file);
					return 0;
				}
				else if (strcmp(input, "undo") == 0) { 
					if (AllBoards->Last == NULL) {
						printf("Nothing to undo\n");
						continue;
					}
					if (AllBoards->Last->Prev == NULL) {
						printf("Nothing to undo\n");
						continue;
					}
					DeleteLastBoard(AllBoards);
					DeleteLastEntry(MoveList);
					DeleteLastEntry(MoveList);
					if (uFlag) {	
						DeleteLastBoard(AllBoards);
					}
					CopyBoard(AllBoards->Last, MasterBoard);
					DisplayBoard(MasterBoard);
					printf("Undo successful!\n");
					uFlag = true;
					continue;
				}
				else if (strcmp(input, "hint") == 0) {
					HintMove = FindBestMove(MasterBoard, activePlayer, 3);
					printf("Move ");                
					printf("%c", HintMove->Vector->Startpoint->x + 97); 
					printf("%d", HintMove->Vector->Startpoint->y + 1);
					printf("%c", HintMove->Vector->Endpoint->x + 97);
					printf("%d", HintMove->Vector->Endpoint->y + 1);
					printf(".\n");                  
					DeleteMoveEntry(HintMove);
					continue;
				}	
				else {
					input[0] = tolower(input[0]) - 97;
					input[2] = tolower(input[2]) - 97;
	
		            x1 = input[0];
		            y1 = input[1] - 49;
		            x2 = input[2];
		            y2 = input[3] - 49;	
			
					if (x1<0 || x1>7 || y1<0 || y1>7 || x2<0 || x2>7 || y2<0 || y1>7) {	
						printf("Invalid Move!\n");			
						continue;
					}
	
					if ((MasterBoard->game_board[x1][y1]->color == 'w' ? true : false) != activePlayer) {
						printf("Invalid Move!\n");
						continue;
					}
			
					MoveVector *MoveVector1 = NewMoveVector(MasterBoard, x1, y1, x2, y2);

					if (MoveVector1 == NULL) {
						printf("Invalid move!\n");
		                continue;
	        	    }
	        	    else {
	                	AppendMove(MoveList, MoveVector1);
	              	    CCBoard = CreateBoard();
					    CopyBoard(MasterBoard, CCBoard);
					    AppendBoard(AllBoards, CCBoard);
					    UpdateBoard(MasterBoard, MoveList->Last);
	                	input[0] = x1 + 97;
	    			    input[1] = y1 + 49;
				    	input[2] = x2 + 97;
		       		    input[3] = y2 + 49;
		      		    fprintf(file, "%lu. %s  ", MoveList->ListLength + 1, input);
					}
				}
			}
		}
		if(Check(MasterBoard, true))
			printf("~WHITE IN CHECK~\n");
		if(Check(MasterBoard, false))
			printf("~BLACK IN CHECK~\n");
		if(CheckMate(MasterBoard, true))
			printf("~WHITE IN CHECK MATE!~\n");
		if(CheckMate(MasterBoard, false))
			printf("~BLACK IN CHECK MATE!~\n");
		activePlayer = !activePlayer;
		uFlag = false;
	}

	DisplayBoard(MasterBoard);
	bool cm1 = CheckMate(MasterBoard, true);
	bool cm2 = CheckMate(MasterBoard, false);
	if(/*RepetitiveMoveThree(MoveList)*/ false)
		printf("Game over! It's a draw!\n");
	else if(cm1 && cm2)
		printf("Game over! It was a draw!\n");
	else if(cm1)
		printf("Game over! The winner is Player 1!\n");
	else if(cm2)
		printf("Game over! The winner is Player 2!\n");
	else if (WhoWon(MasterBoard) == 1) {
		strcpy(winner, "Player1");
		printf("Game over! The winner is %s!\n", winner);
	}
	else {
		strcpy(winner, "Player2");
		printf("Game over! The winner is %s!\n", winner);
	}
	
	DeleteMoveList(MoveList);
	MoveList = NULL;
	DeleteBoard(MasterBoard);
	DeleteBoardList(AllBoards);
	MasterBoard = NULL;

	fclose(file);

	return 0;
}

void PrintHorizontalBorder(){

	printf("  +----+----+----+----+----+----+----+----+\n");
}

void DisplayBoard(Board *GameBoard) {

	char DisplayingBoard[41][8];
	bool resetFlag = false;	

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

					break;
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
	
		PrintHorizontalBorder();
		printf("%d ", j+1);

		for (int i = 0; i < 41; i++) {
			if(DisplayingBoard[i][j] == 'w')
			{
				resetFlag = true;
				printf(ASCII_GREEN);	
				i = i+1;
			}
			else if(DisplayingBoard[i][j] == 'b')
			{
				resetFlag = true;
				printf(ASCII_BLUE);	
				i = i+1;
			}			
			printf("%c", DisplayingBoard[i][j]);
			if(resetFlag)
			{
				printf(ASCII_RESET);
				printf(" ");
				resetFlag = false;
			}
		}
		printf("\n");
	}

	PrintHorizontalBorder();
	printf("    a    b    c    d    e    f    g    h    \n");

}

// return 1 if white player won, return 0 if black player won
int WhoWon(Board *GameBoard) {

	int totalScore = 0;

	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 8; i++) {
			totalScore += GameBoard->game_board[i][j]->val;
		}
	}

	if (totalScore > 0) {
		return 1;
	}
	else {
		return 0;
	}
}

void SaveFile(MoveList *MoveList, char *fname) {
	FILE *save;
	MoveEntry *tempEntry;
	if (MoveList == NULL)
		return;
	if (MoveList->First == NULL)
		return;
	tempEntry = MoveList->First;
	char temp_input[4];

	strcat(fname, ".txt");
	save = fopen(fname, "w");

	while (tempEntry != NULL) {
		temp_input[0] = tempEntry->Vector->Startpoint->x + 97;
		temp_input[1] = tempEntry->Vector->Startpoint->y + 49;
		temp_input[2] = tempEntry->Vector->Endpoint->x + 97;
		temp_input[3] = tempEntry->Vector->Endpoint->y + 49;
		for (int i = 0; i < 4; i++) {
			fprintf(save, "%c", temp_input[i]);
		}
		fprintf(save, "\n");
		tempEntry = tempEntry->Next;
	}

	printf("Current game saved! \"%s\"\n", fname);
    fclose(save);
}

Board *LoadFile(MoveList *MoveList, char *fname) {
	FILE *load, *file;
	Board *GameBoard = CreateBoard();
	char read_input[4];
	int x1, y1, x2, y2;
	char trash;
	InitializeBoard(GameBoard);
	bool activePlayer = true;
	short int turnCount = 0;

	load = fopen(fname, "r");
	file = fopen("log.txt", "w");
	if (load == NULL) {
		printf("Not an existing file!\n");
		return NULL;
	}

	while (!feof(load)) {
		read_input[0] = fgetc(load);
		read_input[0] = tolower(read_input[0]) - 97;
		x1 = read_input[0];

		read_input[1] = fgetc(load);
		y1 = read_input[1] - 49;

		read_input[2] = fgetc(load);
		read_input[2] = tolower(read_input[2]) - 97;
		x2 = read_input[2];

		read_input[3] = fgetc(load);
		y2 = read_input[3] - 49;
		
		if (!feof(load))
			trash = fgetc(load);
		if (x1 > 8 || x1 < 0 || y1 > 8 || y1 < 0|| x2 > 8 || x2 < 0 ||  y2 > 8 || y2 < 0) {
			break;
		turnCount++;
		}
		activePlayer = !turnCount%2;
		GameBoard->currPlayer = activePlayer;
		MoveVector *MoveVector1 = NewMoveVector(GameBoard, x1, y1, x2, y2);	
		MoveVector1->AI = true;
		if ((MoveVector1->Endpoint->y == 0 || MoveVector1->Endpoint->y == 7) && (GameBoard->game_board[x1][y1]->type == 1)) {
			MoveVector1->Promotion = true;
			MoveVector1->AI = true;
		}
		AppendMove(MoveList, MoveVector1);
		UpdateBoard(GameBoard, MoveList->Last);
		read_input[0] = x1 + 97;
		read_input[1] = y1 + 49;
		read_input[2] = x2 + 97;
		read_input[3] = y2 + 49;
		fprintf(file, "%lu. %s  ", MoveList->ListLength + 1, read_input);
	}
	printf("%s loaded successfully!\n", fname);
	fclose(load);
	fclose(file);
	return GameBoard;
}



/*
void DisplayGUI(Board *GameBoard) {

	SDL_Surface *screen;	//This pointer will reference the backbuffer
	//	SDL_Surface *image;	//This pointer will reference our bitmap sprite
	//	SDL_Surface *temp;	//This pointer will temporarily reference our bitmap sprite
	//	SDL_Rect src, dest;	//These rectangles will describe the source and destination regions of our blit
	//	int x;
	int grid[8] = {10, 60, 110, 160, 210, 260, 310, 360};

		//We must first initialize the SDL video component, and check for success
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
		
	//When this program exits, SDL_Quit must be called
	atexit(SDL_Quit);
		
	//Set the video mode to a window of size 320 by 240 pixels with 32 bits per pixel
	screen = SDL_SetVideoMode(640, 480, 32, SDL_ANYFORMAT);
	if (screen == NULL) {
		printf("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
	}

	//Create 2 colors
	Uint32 c1 = SDL_MapRGB(screen->format, 153, 220, 228);
	Uint32 c2 = SDL_MapRGB(screen->format, 82, 85, 85);

	// Rectangle structures for Board

	SDL_Rect board [8][8];

	for (int x = 0; x < 8; x++) {
	    for (int y = 0; y < 8; y++) {
	      	// create Rect
		    board[x][y].x = grid[x];
		    board[x][y].y = grid[y];
		    board[x][y].w = WIDTH;
		    board[x][y].h = HEIGHT;
		    // set fill color
		    if (x % 2 == 0) {    // even row
		        if (y % 2 == 0) {    // even column
		          	// light color
		          	SDL_FillRect(screen, &board[x][y], c1);
		          	SDL_UpdateRect(screen, board[x][y].x, board[x][y].y, board[x][y].w, board[x][y].h);
		        }
		        else {    // odd column
		         	// dark color
		          	SDL_FillRect(screen, &board[x][y], c2);
		          	SDL_UpdateRect(screen, board[x][y].x, board[x][y].y, board[x][y].w, board[x][y].h);
		        }
		    }
		    else {    // odd row
		        if (y % 2 == 0) {    // even column
		          	// dark color
		          	SDL_FillRect(screen, &board[x][y], c2);
		          	SDL_UpdateRect(screen, board[x][y].x, board[x][y].y, board[x][y].w, board[x][y].h);
		        }
		        else {    // odd column
		         	// light color
		          	SDL_FillRect(screen, &board[x][y], c1);
		          	SDL_UpdateRect(screen, board[x][y].x, board[x][y].y, board[x][y].w, board[x][y].h);
		        }
		    }
	    }
	}


	for (int j = 7; j >= 0; j--) {
		for (int i = 7; i >= 0; i--) {

			switch (GameBoard->game_board[i][j]->type) {
				case PAWN: 
					// Piece is white
					if (GameBoard->game_board[i][j]->color == 'w') {
						SDL_BlitSurface(pawn_w_image, &src, screen, &dest);
					}
					else { // Piece is black
						SDL_BlitSurface(pawn_b_image, &src, screen, &dest);
					}

					break;
				case ROOK:
					// Piece is white
					if (GameBoard->game_board[i][j]->color == 'w') {
						SDL_BlitSurface(rook_w_image, &src, screen, &dest);
					}
					else { // Piece is black
						SDL_BlitSurface(rook_b_image, &src, screen, &dest);
					}

					break;
				case KNIGHT:
					// Piece is white
					if (GameBoard->game_board[i][j]->color == 'w') {
						SDL_BlitSurface(knight_w_image, &src, screen, &dest);
					}
					else { // Piece is black
						SDL_BlitSurface(knight_b_image, &src, screen, &dest);
					}

					break;
				case BISHOP:
					// Piece is white
					if (GameBoard->game_board[i][j]->color == 'w') {
						SDL_BlitSurface(bishop_w_image, &src, screen, &dest);
					}
					else { // Piece is black
						SDL_BlitSurface(bishop_b_image, &src, screen, &dest);
					}

					break;
				case QUEEN:
					// Piece is white
					if (GameBoard->game_board[i][j]->color == 'w') {
						SDL_BlitSurface(queen_w_image, &src, screen, &dest);
					}
					else {	// Piece is black
						SDL_BlitSurface(queen_b_image, &src, screen, &dest);
					}

					break;
				case KING:
					// Piece is white
					if (GameBoard->game_board[i][j]->color == 'w') {
						SDL_BlitSurface(king_w_image, &src, screen, &dest);
					}
					else { // Piece is black
						SDL_BlitSurface(king_b_image, &src, screen, &dest);
					}

					break;
				default:

					break;
			}
			board[x][y].x = i * grid[i];
			board[x][y].y = j * grid[j];
			board[x][y].w = // piece width
			board[x][y].h = // piece height
			SDL_UpdateRect(screen, board[x][y].x, board[x][y].y, board[x][y].w, board[x][y].h);
		}
	}

}
*/


