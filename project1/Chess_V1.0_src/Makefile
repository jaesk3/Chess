##################   MAKEFILE   ##################
#Jae		~Chess Please~			 #
##################################################

#########     Variables for targets   ############

FLAGS = -ansi -std=c99 -Wall
CFLAGS = -c $(FLAGS) 
VERSION = 1.0
DVERSION = 1.0D
DEBUG = -DDEBUG -g
DEMO = -DTWOPLAY
INPATH = ./src/
OUTPATH = ./bin/

##################################################

#Target to make all
all: $(INPATH)Chess.o $(INPATH)Opponent.o $(INPATH)Move.o $(INPATH)Main.o
	gcc $(INPATH)Chess.o $(INPATH)Opponent.o $(INPATH)Move.o $(INPATH)Main.o -o $(OUTPATH)ChessPlease_$(VERSION)

#Target to make GUI version
gui: $(INPATH)Chess.o $(INPATH)Opponent.o $(INPATH)Move.o $(INPATH)MainGui.o
	gcc -lSDL $(INPATH)Chess.o $(INPATH)Opponent.o $(INPATH)Move.o $(INPATH)MainGui.o -o $(OUTPATH)ChessPlease_$(VERSION)GUI

#Target to make MainGui.o
MainGui.o: $(INPATH)MainGui.c $(INPATH)Chess.h $(INPATH)Opponent.h $(INPATH)Move.h
	gcc $(CFLAGS) $(INPATH)MainGui.c -o $(INPATH)MainGui.o

#Target to make test 
test: $(INPATH)ChessTest.o $(INPATH)OpponentTest.o $(INPATH)MoveTest.o $(INPATH)MainTest.o $(INPATH)ListAvailableMoves.o
	gcc $(INPATH)ChessTest.o $(INPATH)OpponentTest.o $(INPATH)MoveTest.o $(INPATH)MainTest.o -o $(OUTPATH)ChessPlease_$(DVERSION)

#Target for Chess.o
$(INPATH)Chess.o: $(INPATH)Chess.c $(INPATH)Chess.h
	gcc $(CFLAGS) $(INPATH)Chess.c -o $(INPATH)Chess.o

#Target for ChessTest.o
$(INPATH)ChessTest.o: $(INPATH)Chess.c $(INPATH)Chess.h
	gcc $(DEBUG) $(CFLAGS) $(INPATH)Chess.c -o $(INPATH)ChessTest.o

#Target for Opponent.o
$(INPATH)Opponent.o: $(INPATH)Opponent.c $(INPATH)Opponent.h $(INPATH)Chess.h $(INPATH)Move.h
	gcc $(CFLAGS) $(INPATH)Opponent.c -o $(INPATH)Opponent.o

#Target for OpponentTest.o
$(INPATH)OpponentTest.o: $(INPATH)Opponent.c $(INPATH)Opponent.h $(INPATH)Chess.h $(INPATH)Move.h
	gcc $(DEBUG) $(CFLAGS) $(INPATH)Opponent.c -o $(INPATH)OpponentTest.o

#Target for Move.o
$(INPATH)Move.o: $(INPATH)Move.c $(INPATH)Move.h $(INPATH)Chess.h
	gcc $(CFLAGS) $(INPATH)Move.c -o $(INPATH)Move.o

#Target for MoveTest.o
$(INPATH)MoveTest.o: $(INPATH)Move.c $(INPATH)Move.h $(INPATH)Chess.h
	gcc $(DEBUG) $(CFLAGS) $(INPATH)Move.c -o $(INPATH)MoveTest.o

#Target for Main.o
$(INPATH)Main.o: $(INPATH)Main.c $(INPATH)Chess.h $(INPATH)Opponent.h $(INPATH)Move.h
	gcc $(CFLAGS) $(INPATH)Main.c -o $(INPATH)Main.o

#Target for MainTest.o
$(INPATH)MainTest.o: $(INPATH)Main.c $(INPATH)Chess.h $(INPATH)Opponent.h $(INPATH)Move.h
	gcc $(DEBUG) $(CFLAGS) $(INPATH)Main.c -o $(INPATH)MainTest.o

#Target to make clean:
clean:
	rm -f $(INPATH)*.o 
	rm -f $(OUTPATH)ChessPlease_$(VERSION)
	rm -f $(OUTPATH)ChessPlease_$(DVERSION)
	rm -f ChessPlease_$(DVERSION)

