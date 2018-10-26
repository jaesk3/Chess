#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "Move.h"
#include "Chess.h"
#include "Opponent.h"
#include <assert.h>

#include "SDL/SDL.h"


#define ASCII_RED "\x1b[31m"
#define ASCII_GREEN "\x1b[32m"
#define ASCII_BLUE "\x1b[34m"
#define ASCII_RESET "\x1b[0m"

//#define WAIT      // wait some time to show the image
#define EVENTS      // process events and wait for window close
#define WIDTH 100
#define HEIGHT 100
#define OFFSET 10

#define ASCII_DISPLAY
//#define GUI_DISPLAY





void DisplayBoard(Board *GameBoard);
void PrintHorizontalBorder();
void SaveFile(MoveList *List, char *fname);
Board *LoadFile(MoveList *List, char *fname);
int WhoWon(Board *GameBoard);


void FillBoardSpace(int x, int y, SDL_Rect board [][8], Uint32 light, Uint32 dark, SDL_Surface * screen);
void LoadBMPImage(char * filename, Uint32 greenScreen, SDL_Surface * image, SDL_Surface * screen, SDL_Rect board[][8], SDL_Surface * pieceImages[][8], int x, int y, SDL_Surface * temp) ;



int main() {
    
    
    
    
    
    Board *GameBoard = NULL;
    MoveList *MoveList = CreateMoveList();
    char colorInput, winner[7];
    bool activePlayer = true; // If 1, then Player1's turn. If 0, then Player2's turn
    bool realPlayer = false;
    char input[5];
    char fname[20];
    int x, y;
    int ax = -1;
    int bx = -1;
    int ay = -1;
    int by = -1;
    int x1, y1, x2, y2;
    unsigned char pMode = 0;
    char loadFile, saveInput;
    FILE *file;
    char difficulty;
    char mode = 'a';
    int searchLevel = 5;
    
    printf("-----Welcome to Chess Please!-----\n");
    printf("Do you want to load previous game? (y/n) : ");
    scanf("%c", &loadFile);
    
    if (loadFile == 'y') {
        file = fopen("log.txt", "a");//append to previously written file
        while (GameBoard == NULL) {
            printf("Enter file name to load (example.txt) : ");
            scanf("%s", fname);
            GameBoard = LoadFile(MoveList, fname);
            SetPlayer('W', false, GameBoard->Player1);
            SetPlayer('B', true, GameBoard->Player2);
            realPlayer = true;
            pMode = 1;
            activePlayer = GameBoard->currPlayer;
        }
    }
    else {
        file = fopen("log.txt", "w+");
        
        printf("Please choose your color : \n");
        printf("\tW = White\tB = Black\t");
        scanf(" %c", &colorInput);
        
        GameBoard = CreateBoard();
        if (colorInput == 'W' || colorInput == 'w') {
            SetPlayer('W', false, GameBoard->Player1);
            SetPlayer('B', true, GameBoard->Player2);
            realPlayer = true;
        }
        else if(colorInput == 'B' || colorInput == 'b') {
            SetPlayer('W', true, GameBoard->Player1);
            SetPlayer('B', false, GameBoard->Player2);
        }
        
        printf("Play in (2) player mode or (1) player mode?\t");
        scanf("%hhu", &pMode);
        
        printf("Mode (A)scii or (G)UI?\n");
        scanf(" %c", &mode);
        switch(mode)
        {
            case 'A':
            case 'a':
                // #define ASCII_DISPLAY
                mode = 'a';
                break;
            case 'G':
            case 'g':
                // #define GUI_DISPLAY
                mode = 'g';
                break;
                
            default:
                break;
        }
        
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
                searchLevel = 5;
                break;
            default:
                break;
        }
        InitializeBoard(GameBoard);
    }
    
    
    
    
    
    //#ifdef ASCII_DISPLAY
    if (mode == 'a') {
        
        // Game starts
        while (!isGameOver(GameBoard) && !(CheckMate(GameBoard, true))) {
            
            DisplayBoard(GameBoard);
            
            // If the user inputs 'w', he/she goes first
            if (activePlayer == realPlayer) {
                printf("--Type \"save\" to save current game--\n");
                printf("--Type \"quit\" to quit current game--\n");
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
                    exit(1);
                }
                else {
                    input[0] = tolower(input[0]) - 97;
                    input[2] = tolower(input[2]) - 97;
                    
                    x1 = input[0];
                    y1 = input[1] - 49;
                    x2 = input[2];
                    y2 = input[3] - 49;
                    
                    if(x1<0 || x1>7 || y1<0 || y1>7 || x2<0 || x2>7 || y2<0 || y1>7)
                    {
                        printf("Invalid Move!\n");
                        continue;
                    }
                    
                    if((GameBoard->game_board[x1][y1]->color == 'w' ? true : false) != activePlayer)
                    {
                        printf("Invalid Move!\n");
                        continue;
                    }
                    
                    MoveVector *MoveVector1 = NewMoveVector(GameBoard, x1, y1, x2, y2);
                    
                    if (MoveVector1 == NULL) {
                        printf("Invalid move!\n");
                        continue;
                    }
                    else {
                        AppendMove(MoveList, MoveVector1);
                        UpdateBoard(GameBoard, MoveList->Last);
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
                    MoveEntry *MoveEntry1 = FindBestMove(GameBoard, activePlayer, searchLevel);
                    UpdateBoard(GameBoard, MoveEntry1);
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
                        exit(1);
                    }
                    else {
                        input[0] = tolower(input[0]) - 97;
                        input[2] = tolower(input[2]) - 97;
                        
                        x1 = input[0];
                        y1 = input[1] - 49;
                        x2 = input[2];
                        y2 = input[3] - 49;
                        
                        if(x1<0 || x1>7 || y1<0 || y1>7 || x2<0 || x2>7 || y2<0 || y1>7)
                        {
                            printf("Invalid Move!\n");
                            continue;
                        }
                        
                        if((GameBoard->game_board[x1][y1]->color == 'w' ? true : false) != activePlayer)
                        {
                            printf("Invalid Move!\n");
                            continue;
                        }
                        
                        MoveVector *MoveVector1 = NewMoveVector(GameBoard, x1, y1, x2, y2);
                        
                        if (MoveVector1 == NULL) {
                            printf("Invalid move!\n");
                            continue;
                        }
                        else {
                            AppendMove(MoveList, MoveVector1);
                            UpdateBoard(GameBoard, MoveList->Last);
                            input[0] = x1 + 97;
                            input[1] = y1 + 49;
                            input[2] = x2 + 97;
                            input[3] = y2 + 49;
                            fprintf(file, "%lu. %s  ", MoveList->ListLength + 1, input);
                            
                        }
                    }
                }
            }
            
            activePlayer = !activePlayer;
            
        }
        
        DisplayBoard(GameBoard);
        bool cm1 = CheckMate(GameBoard, true);
        bool cm2 = CheckMate(GameBoard, false);
        if(cm1 && cm2)
            printf("Game over! It was a draw!\n");
        else if(cm1)
            printf("Game over! The winner is Player 1!\n");
        else if(cm2)
            printf("Game over! The winner is Player 2!\n");
        else if (WhoWon(GameBoard) == 1) {
            strcpy(winner, "Player1");
        }
        else {
            strcpy(winner, "Player2");
        }
        printf("Game over! The winner is %s!\n", winner);
        
        DeleteMoveList(MoveList);
        MoveList = NULL;
        DeleteBoard(GameBoard);
        GameBoard = NULL;
        
        fclose(file);
        
        return 0;
        
    }
    
    
    //#endif
    
    
    
    
    
    
    //#ifdef GUI_DISPLAY
    
    else if (mode == 'g') {
        
        SDL_Surface *screen;    //This pointer will reference the backbuffer
        //  SDL_Surface * gameBoard = NULL;
        Uint32 greenScreen;
        
        //Creating piece bmps
        SDL_Surface * blackPawns[8];
        SDL_Surface * whitePawns[8];
        
        SDL_Surface * wKing;
        SDL_Surface * wQueen;
        SDL_Surface * wRooks[2];
        SDL_Surface * wKnights[2];
        SDL_Surface * wBishops[2];
        
        SDL_Surface * bKing;
        SDL_Surface * bQueen;
        SDL_Surface * bRooks[2];
        SDL_Surface * bKnights[2];
        SDL_Surface * bBishops[2];
        SDL_Surface * empty;
        SDL_Surface * temp = NULL;
        
        SDL_Surface * pieceImages[8][8];
        
        
#ifdef EVENTS
        SDL_Event event;    /* Event structure */
        int Running;
#endif
        
        
        // Screen Declaration
        //We must first initialize the SDL video component, and check for success
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            printf("Unable to initialize SDL: %s\n", SDL_GetError());
            return 1;
        }
        
        //When this program exits, SDL_Quit must be called
        atexit(SDL_Quit);
        
        //Set the video mode to a window of size 320 by 240 pixels with 32 bits per pixel
        screen = SDL_SetVideoMode(850, 850, 32, SDL_ANYFORMAT /*SDL_DOUBLEBUF | SDL_FULLSCREEN*/);
        if (screen == NULL) {
            printf("Unable to set video mode: %s\n", SDL_GetError());
            return 1;
        }
        
        
        
        
        
        
        
        // Variable declaration
        int grid[8] = {0 + OFFSET, 100 + OFFSET, 200 + OFFSET, 300 + OFFSET, 400 + OFFSET, 500 + OFFSET, 600 + OFFSET, 700 + OFFSET};
        Uint32 c1 = SDL_MapRGB(screen->format, 153, 220, 228);   // light blue
        Uint32 c2 = SDL_MapRGB(screen->format, 82, 85, 85);      // dark grey
        //  Uint32 c3 = SDL_MapRGB(screen->format, 204, 239, 134);   // light green
        
        SDL_Rect board [8][8];
        int clickFlag = 0;     // to know if mouseButtonDown event has been used for the first time
        
        
        
        
        
        // Rectangle structures for Board
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                // create Rect
                board[x][y].x = grid[x];
                board[x][y].y = grid[y];
                board[x][y].w = WIDTH;
                board[x][y].h = HEIGHT;
                FillBoardSpace(x, y, board, c1, c2, screen);
            }
        }
        
        
        
        
        
        // Adding pieces to board...
        
        /*WE WILL HAVE TO CHANGE THE NAMES OF THE BMPS TO DIFFER FROM BLACK AND WHITE*/
        
        //ADDING BLACK PAWNS
        temp = SDL_LoadBMP("wPawnKey.bmp");
        for (int x = 0; x < 8; x++) {
            
            
            
            blackPawns[x] = SDL_DisplayFormat(temp);
            pieceImages[x][1] = blackPawns[x];
            greenScreen = SDL_MapRGB(blackPawns[x]->format, 255, 255, 255);   // green screen
            SDL_SetColorKey(blackPawns[x], SDL_SRCCOLORKEY, greenScreen);
            SDL_BlitSurface(blackPawns[x], NULL, screen, &board[x][1]);
            SDL_UpdateRect(screen, board[x][1].x, board[x][1].y, board[x][1].w, board[x][1].h);
            
            /*
             
             
             LoadBMPImage("wPawnKey.bmp", greenScreen, blackPawns[x], screen, board, pieceImages, x, 1, temp);
             */
            
        }
        
        //ADDING WHITE PAWNS
        temp = SDL_LoadBMP("bPawnKey.bmp");
        for (int x = 0; x < 8; x++) {
            whitePawns[x] = SDL_DisplayFormat(temp);
            pieceImages[x][6] = whitePawns[x];
            greenScreen = SDL_MapRGB(whitePawns[x]->format, 255, 255, 255);   // green screen
            SDL_SetColorKey(whitePawns[x], SDL_SRCCOLORKEY, greenScreen);
            SDL_BlitSurface(whitePawns[x], NULL, screen, &board[x][6]);
            SDL_UpdateRect(screen, board[x][6].x, board[x][6].y, board[x][6].w, board[x][6].h);
        }
        
        //ADDING BLACK QUEEN AND KING
        temp = SDL_LoadBMP("wKingKey.bmp");
        bKing = SDL_DisplayFormat(temp);
        pieceImages[4][0] = bKing;
        greenScreen = SDL_MapRGB(bKing->format, 255, 255, 255);   // green screen
        SDL_SetColorKey(bKing, SDL_SRCCOLORKEY, greenScreen);
        SDL_BlitSurface(bKing, NULL, screen, &board[4][0]);
        
        temp = SDL_LoadBMP("wQueenKey.bmp");
        bQueen = SDL_DisplayFormat(temp);
        pieceImages[3][0] = bQueen;
        greenScreen = SDL_MapRGB(bQueen->format, 255, 255, 255);   // green screen
        SDL_SetColorKey(bQueen, SDL_SRCCOLORKEY, greenScreen);
        SDL_BlitSurface(bQueen, NULL, screen, &board[3][0]);
        
        //ADDING BLACK ROOKS, KNIGHTS, AND BISHOPS
        temp = SDL_LoadBMP("wRookKey.bmp");
        
        //rook
        bRooks[0] = SDL_DisplayFormat(temp);
        pieceImages[0][0] = bRooks[0];
        greenScreen = SDL_MapRGB(bRooks[0]->format, 255, 255, 255);   // green screen
        SDL_SetColorKey(bRooks[0], SDL_SRCCOLORKEY, greenScreen);
        SDL_BlitSurface(bRooks[0], NULL, screen, &board[0][0]);
        
        bRooks[1] = SDL_DisplayFormat(temp);
        pieceImages[7][0] = bRooks[1];
        greenScreen = SDL_MapRGB(bRooks[1]->format, 255, 255, 255);   // green screen
        SDL_SetColorKey(bRooks[1], SDL_SRCCOLORKEY, greenScreen);
        SDL_BlitSurface(bRooks[1], NULL, screen, &board[7][0]);
        
        temp = SDL_LoadBMP("wBishopKey.bmp");
        //bishop
        bBishops[0] = SDL_DisplayFormat(temp);
        pieceImages[2][0] = bBishops[0];
        greenScreen = SDL_MapRGB(bBishops[0]->format, 255, 255, 255);   // green screen
        SDL_SetColorKey(bBishops[0], SDL_SRCCOLORKEY, greenScreen);
        SDL_BlitSurface(bBishops[0], NULL, screen, &board[2][0]);
        
        bBishops[1] = SDL_DisplayFormat(temp);
        pieceImages[5][0] = bBishops[1];
        greenScreen = SDL_MapRGB(bBishops[1]->format, 255, 255, 255);   // green screen
        SDL_SetColorKey(bBishops[1], SDL_SRCCOLORKEY, greenScreen);
        SDL_BlitSurface(bBishops[1], NULL, screen, &board[5][0]);
        
        temp = SDL_LoadBMP("wKnightKey.bmp");
        //knight
        bKnights[0] = SDL_DisplayFormat(temp);
        pieceImages[1][0] = bKnights[0];
        greenScreen = SDL_MapRGB(bKnights[0]->format, 255, 255, 255);   // green screen
        SDL_SetColorKey(bKnights[0], SDL_SRCCOLORKEY, greenScreen);
        SDL_BlitSurface(bKnights[0], NULL, screen, &board[1][0]);
        
        bKnights[1] = SDL_DisplayFormat(temp);
        pieceImages[6][0] = bKnights[1];
        greenScreen = SDL_MapRGB(bKnights[1]->format, 255, 255, 255);   // green screen
        SDL_SetColorKey(bKnights[1], SDL_SRCCOLORKEY, greenScreen);
        SDL_BlitSurface(bKnights[1], NULL, screen, &board[6][0]);
        
        //ADDING WHITE QUEEN AND KING
        temp = SDL_LoadBMP("bKingKey.bmp");
        wKing = SDL_DisplayFormat(temp);
        pieceImages[4][7] = wKing;
        greenScreen = SDL_MapRGB(wKing->format, 255, 255, 255);   // green screen
        SDL_SetColorKey(wKing, SDL_SRCCOLORKEY, greenScreen);
        SDL_BlitSurface(wKing, NULL, screen, &board[4][7]);
        
        temp = SDL_LoadBMP("bQueenKey.bmp");
        wQueen = SDL_DisplayFormat(temp);
        pieceImages[3][7] = wQueen;
        greenScreen = SDL_MapRGB(wQueen->format, 255, 255, 255);   // green screen
        SDL_SetColorKey(wQueen, SDL_SRCCOLORKEY, greenScreen);
        SDL_BlitSurface(wQueen, NULL, screen, &board[3][7]);
        
        //ADDING WHITE ROOKS, KNIGHTS, AND BISHOPS
        temp = SDL_LoadBMP("bRookKey.bmp");
        
        //rook
        wRooks[0] = SDL_DisplayFormat(temp);
        pieceImages[0][7] = wRooks[0];
        greenScreen = SDL_MapRGB(wRooks[0]->format, 255, 255, 255);   // green screen
        SDL_SetColorKey(wRooks[0], SDL_SRCCOLORKEY, greenScreen);
        SDL_BlitSurface(wRooks[0], NULL, screen, &board[0][7]);
        
        wRooks[1] = SDL_DisplayFormat(temp);
        pieceImages[7][7] = wRooks[1];
        greenScreen = SDL_MapRGB(wRooks[1]->format, 255, 255, 255);   // green screen
        SDL_SetColorKey(wRooks[1], SDL_SRCCOLORKEY, greenScreen);
        SDL_BlitSurface(wRooks[1], NULL, screen, &board[7][7]);
        
        temp = SDL_LoadBMP("bBishopKey.bmp");
        //bishop
        wBishops[0] = SDL_DisplayFormat(temp);
        pieceImages[2][7] = wBishops[0];
        greenScreen = SDL_MapRGB(wBishops[0]->format, 255, 255, 255);   // green screen
        SDL_SetColorKey(wBishops[0], SDL_SRCCOLORKEY, greenScreen);
        SDL_BlitSurface(wBishops[0], NULL, screen, &board[2][7]);
        
        wBishops[1] = SDL_DisplayFormat(temp);
        pieceImages[5][7] = wBishops[1];
        greenScreen = SDL_MapRGB(wBishops[1]->format, 255, 255, 255);   // green screen
        SDL_SetColorKey(wBishops[1], SDL_SRCCOLORKEY, greenScreen);
        SDL_BlitSurface(wBishops[1], NULL, screen, &board[5][7]);
        
        temp = SDL_LoadBMP("bKnightKey.bmp");
        //knight
        wKnights[0] = SDL_DisplayFormat(temp);
        pieceImages[1][7] = wKnights[0];
        greenScreen = SDL_MapRGB(wKnights[0]->format, 255, 255, 255);   // green screen
        SDL_SetColorKey(wKnights[0], SDL_SRCCOLORKEY, greenScreen);
        SDL_BlitSurface(wKnights[0], NULL, screen, &board[1][7]);
        
        wKnights[1] = SDL_DisplayFormat(temp);
        pieceImages[6][7] = wKnights[1];
        greenScreen = SDL_MapRGB(wKnights[1]->format, 255, 255, 255);   // green screen
        SDL_SetColorKey(wKnights[1], SDL_SRCCOLORKEY, greenScreen);
        SDL_BlitSurface(wKnights[1], NULL, screen, &board[6][7]);
        
        //EMPTY SPACES
        temp = SDL_LoadBMP("EMPTY.bmp");
        for(int x = 0; x < 8; x++){
            for(int y = 2; y < 6; y++){
                empty = SDL_DisplayFormat(temp);
                pieceImages[x][y] = empty;
                greenScreen = SDL_MapRGB(empty->format, 255, 255, 255);   // green screen
                SDL_SetColorKey(empty, SDL_SRCCOLORKEY, greenScreen);
                SDL_BlitSurface(empty, NULL, screen, &board[x][y]);
                
            }
        }
        
        //TRYING JUST ADDING FOR LOOP TO UPDATE THE RECT, DON'T KNOW IF IT HAS TO BE RIGHT AFTER BLIT SURFACE FOR EACH ONE
        for(int x = 0; x < 8; x++){
            for(int y = 0; y < 8; y++){
                SDL_UpdateRect(screen, board[x][y].x, board[x][y].y, board[x][y].w, board[x][y].h);
            }
        }
        
        
        
        
        
        
        //Process events
        Running = 1;
        printf("Running!\n");
        while(Running && SDL_WaitEvent(&event))
        {
            
            
            
            
            
            switch(event.type){
                    
                case SDL_MOUSEBUTTONDOWN:
                {
                    x = event.button.x;
                    y = event.button.y;
                    
                    printf("X Coordinate: %d\n", x);
                    printf("Y Coordinate: %d\n\n", y);
                    
                    //int ax, ay, bx, by;
                    
                    if (clickFlag == false && ax == -1 && ay == -1) {
                        if (activePlayer == realPlayer) {
                            ax = (x - OFFSET) / WIDTH;
                            ay = (y - OFFSET) / HEIGHT;
                            printf("ax, ay: %d, %d \n", ax, ay);
                            clickFlag = true;
                        }
                        else {
                            clickFlag = true;
                        }
                    }
                    else if (clickFlag == true && activePlayer == realPlayer) {
                        bx = (x - OFFSET) / WIDTH;
                        by = (y - OFFSET) / HEIGHT;
                        printf("bx, by: %d, %d \n", bx, by);
                        
                        
                        if (activePlayer == realPlayer) {
                            
                            x1 = ax;
                            y1 = ay;
                            x2 = bx;
                            y2 = by;
                            
                            if(x1<0 || x1>7 || y1<0 || y1>7 || x2<0 || x2>7 || y2<0 || y1>7)
                            {
                                printf("Invalid Move!\n");
                                ax = (x - OFFSET) / WIDTH;
                                ay = (y - OFFSET) / HEIGHT;
                                continue;
                            }
                            
                            if((GameBoard->game_board[x1][y1]->color == 'w' ? true : false) != activePlayer)
                            {
                                printf("Invalid Move!\n");
                                ax = (x - OFFSET) / WIDTH;
                                ay = (y - OFFSET) / HEIGHT;
                                continue;
                            }
                            
                            MoveVector *MoveVector1 = NewMoveVector(GameBoard, x1, y1, x2, y2);
                            
                            if (MoveVector1 == NULL) {
                                printf("Invalid move!\n");
                                ax = (x - OFFSET) / WIDTH;
                                ay = (y - OFFSET) / HEIGHT;
                                continue;
                            }
                            else {
                                AppendMove(MoveList, MoveVector1);
                                UpdateBoard(GameBoard, MoveList->Last);
                                input[0] = x1 + 97;
                                input[1] = y1 + 49;
                                input[2] = x2 + 97;
                                input[3] = y2 + 49;
                                fprintf(file, "%lu. %s  ", MoveList->ListLength + 1, input);
                            }
                        }
                        
                        
                        
                        FillBoardSpace(ax, ay, board, c1, c2, screen);
                        FillBoardSpace(bx, by, board, c1, c2, screen);
                        
                        DisplayBoard(GameBoard);
                        
                        /*
                         
                         temp = pieceImages[ax][ay];
                         if (pieceImages[bx][by] != empty) {
                         pieceImages[ax][ay] = empty;
                         }
                         else {
                         pieceImages[ax][ay] = pieceImages[bx][by];
                         }
                         //   pieceImages[ax][ay] = pieceImages[bx][by];
                         pieceImages[bx][by] = temp;
                         
                         */
                        
                        
                        pieceImages[bx][by] = pieceImages[ax][ay];
                        pieceImages[ax][ay] = empty;
                        
                        
                        SDL_BlitSurface(pieceImages[ax][ay], NULL, screen, &board[ax][ay]);
                        SDL_BlitSurface(pieceImages[bx][by], NULL, screen, &board[bx][by]);
                        SDL_Flip(screen);
                        
                        activePlayer = !activePlayer;
                        clickFlag = false;
                        
                        
                    }
                    break;
                }
                case SDL_QUIT:
                {
                    printf("Quit event received!\n");
                    Running = 0;
                    break;
                }
                default: /* ignore any other events */
                {
                    //printf("Oops, unknown event!\n");
                    break;
                }
            } /* hctiws */
            
            if (activePlayer != realPlayer) {
                // If the mode is p vs AI
                if(pMode == 1)
                {
                    MoveEntry *MoveEntry1 = FindBestMove(GameBoard, activePlayer, searchLevel);
                    UpdateBoard(GameBoard, MoveEntry1);
                    AppendMoveEntry(MoveList, MoveEntry1);
                    
                    ax = MoveEntry1->Vector->Startpoint->x;
                    ay = MoveEntry1->Vector->Startpoint->y;
                    bx = MoveEntry1->Vector->Endpoint->x;
                    by = MoveEntry1->Vector->Endpoint->y;
                    input[0] = ax + 97;
                    input[1] = ay + 49;
                    input[2] = bx + 97;
                    input[3] = by + 49;
                    fprintf(file, "%lu. %s  ", MoveList->ListLength + 1, input);
                }
                
                FillBoardSpace(ax, ay, board, c1, c2, screen);
                FillBoardSpace(bx, by, board, c1, c2, screen);
                
                DisplayBoard(GameBoard);
                
                
                /*
                 temp = pieceImages[ax][ay];
                 if (pieceImages[bx][by] != empty) {
                 pieceImages[ax][ay] = empty;
                 }
                 else {
                 pieceImages[ax][ay] = pieceImages[bx][by];
                 }
                 //   pieceImages[ax][ay] = pieceImages[bx][by];
                 pieceImages[bx][by] = temp;
                 */
                
                
                pieceImages[bx][by] = pieceImages[ax][ay];
                pieceImages[ax][ay] = empty;
                
                
                SDL_BlitSurface(pieceImages[ax][ay], NULL, screen, &board[ax][ay]);
                SDL_BlitSurface(pieceImages[bx][by], NULL, screen, &board[bx][by]);
                SDL_Flip(screen);
                
                
                ax = -1;
                ay = -1;
                activePlayer = !activePlayer;
            }
            
            
            
            if (isGameOver(GameBoard) && CheckMate(GameBoard, true)) {
                Running = 0;
            }
            
            
            
        } /* elihw */
        
        DisplayBoard(GameBoard);
        bool cm1 = CheckMate(GameBoard, true);
        bool cm2 = CheckMate(GameBoard, false);
        if(cm1 && cm2)
            printf("Game over! It was a draw!\n");
        else if(cm1)
            printf("Game over! The winner is Player 1!\n");
        else if(cm2)
            printf("Game over! The winner is Player 2!\n");
        else if (WhoWon(GameBoard) == 1) {
            strcpy(winner, "Player1");
        }
        else {
            strcpy(winner, "Player2");
        }
        printf("Game over! The winner is %s!\n", winner);
        
        DeleteMoveList(MoveList);
        MoveList = NULL;
        DeleteBoard(GameBoard);
        GameBoard = NULL;
        
        fclose(file);
        
        
        
        SDL_FreeSurface(temp);
        for (int i = 0; i < 8; i++) {
            SDL_FreeSurface(blackPawns[i]);
        }
        //************************MAKE SURE TO FREE EVERYTHING!!!!!!!!!!!!!!!!!!!!
        
        
        //Return success!
        return 0;
    }
    
    
    //#endif
    
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

void SaveFile(MoveList *List, char *fname) {
    FILE *save;
    MoveEntry *tempEntry;
    tempEntry = List->First;
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

Board *LoadFile(MoveList *List, char *fname) {
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
        
        if(!feof(load))
            trash = fgetc(load);
        if (x1 > 8 || x1 < 0 || y1 > 8 || y1 < 0|| x2 > 8 || x2 < 0 ||  y2 > 8 || y2 < 0) {
            break;
            turnCount++;
        }
        activePlayer = turnCount%2;
        GameBoard->currPlayer = activePlayer;
        MoveVector *MoveVector1 = NewMoveVector(GameBoard, x1, y1, x2, y2);
        AppendMove(List, MoveVector1);
        UpdateBoard(GameBoard, List->Last);
        read_input[0] = x1 + 97;
        read_input[1] = y1 + 49;
        read_input[2] = x2 + 97;
        read_input[3] = y2 + 49;
        fprintf(file, "%lu. %s  ", List->ListLength + 1, read_input);
        
    }
    printf("%s loaded successfully!\n", fname);
    fclose(load);
    fclose(file);
    return GameBoard;
}



void FillBoardSpace(int x, int y, SDL_Rect board [][8], Uint32 light, Uint32 dark, SDL_Surface * screen) {
    
    if (x % 2 == 0) {    // even row
        if (y % 2 == 0) {    // even column
            // light color
            SDL_FillRect(screen, &board[x][y], light);
        }
        else {    // odd column
            // dark color
            SDL_FillRect(screen, &board[x][y], dark);
        }
    }
    else {    // odd row
        if (y % 2 == 0) {    // even column
            // dark color
            SDL_FillRect(screen, &board[x][y], dark);
        }
        else {    // odd column
            // light color
            SDL_FillRect(screen, &board[x][y], light);
        }
    }
    
    SDL_UpdateRect(screen, board[x][y].x, board[x][y].y, board[x][y].w, board[x][y].h);
    
}

void LoadBMPImage(char * filename, Uint32 greenScreen, SDL_Surface * image, SDL_Surface * screen, SDL_Rect board[][8], SDL_Surface * pieceImages[][8], int x, int y, SDL_Surface * temp) {
    
    temp = SDL_LoadBMP(filename);
    image = SDL_DisplayFormat(temp);
    pieceImages[x][y] = image;
    greenScreen = SDL_MapRGB(image->format, 255, 255, 255);   // green screen
    SDL_SetColorKey(image, SDL_SRCCOLORKEY, greenScreen);
    SDL_BlitSurface(image, NULL, screen, &board[x][y]);
    SDL_UpdateRect(screen, board[x][y].x, board[x][y].y, board[x][y].w, board[x][y].h);
    // SDL_FreeSurface(temp);
}
