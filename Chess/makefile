CC=gcc
OBJS=chessSystem.o tournament.o player.o game.o insideFunctions.o tests/chessSystemTestsExample.o
EXEC=chess
DEBUG=# No debug
CFLAGS=-std=c99 -Wall -Werror -pedantic-errors $(DEBUG) 
OBJ = chess.o
$(EXEC) : $(OBJS)
	$(CC) $(DEBUG) $(OBJS) -o $@ -L. -lmap

$(OBJ) : $(OBJS)
	ld -r -o $@ $(OBJS)

chessSystem.o: chessSystem.c chessSystem.h tournament.h map.h player.h \
 insideFunctions.h

game.o: game.c map.h game.h chessSystem.h player.h insideFunctions.h

insideFunctions.o: insideFunctions.c map.h

player.o: player.c tournament.h chessSystem.h map.h player.h game.h \
 insideFunctions.h

tournament.o: tournament.c tournament.h chessSystem.h map.h player.h \
 game.h insideFunctions.h

tests/chessSystemTestsExample.o: tests/chessSystemTestsExample.c \
 tests/../chessSystem.h tests/../test_utilities.h
 
clean:
	rm -f $(OBJS) $(OBJ) $(EXEC)

