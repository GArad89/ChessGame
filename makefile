CC = gcc
OBJS = main.o CHMainAux.o CHGame.o CHArrayList.o CHThreatMap.o CHParser.o CHMinimaxNode.o CHMinimax.o CHGUI.o


CC_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

all: chessprog
chessprog: $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
main.o: main.c CHMainAux.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHMainAux.o: CHMainAux.c CHMainAux.h CHGame.h CHParser.h CHMinimax.h CHGUI.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
CHGame.o: CHGame.c CHGame.h CHArrayList.h CHThreatMap.h
	$(CC) $(COMP_FLAG) -c $*.c
CHThreatMap.o: CHThreatMap.c CHThreatMap.h CHGame.h
	$(CC) $(COMP_FLAG) -c $*.c 
CHArrayList.o: CHArrayList.h CHArrayList.c
	$(CC) $(COMP_FLAG) -c $*.c
CHParser.o: CHParser.c CHParser.h
	$(CC) $(COMP_FLAG) -c $*.c
CHMinimaxNode.o: CHMinimaxNode.c CHMinimaxNode.h CHGame.h
	$(CC) $(COMP_FLAG) -c $*.c
CHMinimax.o: CHMinimax.c CHMinimax.h CHMinimaxNode.h CHGame.h
	$(CC) $(COMP_FLAG) -c $*.c
CHGUI.o: CHGUI.c CHGUI.h CHMainAux.h CHGame.h CHMinimax.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c

clean:
	rm -f *.o chessprog 
