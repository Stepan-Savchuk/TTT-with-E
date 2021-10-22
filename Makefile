.PHONY: all install clean

SRCS = src/TTT.cpp

OBJS = $(SRCS:.cpp=.o)

CC = g++

INCLUDE_PATHS = -ISDL2\include

LIBRARY_PATHS = -LSDL2\lib

COMPILER_FLAGS = -w -Wl,-subsystem,windows

LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -static-libgcc -static-libstdc++ -Wl,-Bstatic -lpthread

EXE_PATH = build

EXE_NAME = $(EXE_PATH)/TTT

all : $(OBJS)
	$(CC) $< $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(EXE_NAME)


$(OBJS) : $(SRCS)
	$(CC) -c $< $(INCLUDE_PATHS) -o $@


install : $(OBJS)
	EXE_PATH=release
	$(CC) $< $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXE_NAME)


clean : 
	rm -rf $(OBJS) $(EXE_NAME)
