.PHONY: all clean release=1

SRCS = src/TTT.cpp

OBJS = $(SRCS:.cpp=.o)

CC = g++

INCLUDE_PATHS = -ISDL2/include

LIBRARY_PATHS = -LSDL2/lib

COMPILER_FLAGS = -w -Wl,-subsystem,windows

LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -static-libgcc -static-libstdc++ -Wl,-Bstatic -lpthread

BINARY_PATH = build

BINARY_NAME = $(BINARY_PATH)/TTT

all : $(OBJS)
	$(CC) $< $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(BINARY_NAME)


$(OBJS) : $(SRCS)
	$(CC) -c $< $(INCLUDE_PATHS) -o $@

clean : 
	rm -rf $(OBJS) $(EXE_NAME)
