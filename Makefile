OBJS = TTT.cpp

CC = g++

INCLUDE_PATHS = -ISDL2\include

LIBRARY_PATHS = -LSDL2\lib

COMPILER_FLAGS = -w -Wl,-subsystem,windows

LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -static-libgcc -static-libstdc++ -Wl,-Bstatic -lpthread

OBJ_NAME = TTT

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME)
