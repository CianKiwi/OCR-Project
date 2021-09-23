
CC=g++
CFLAGS= -g -Wall
SRC=src
BIN=bin
SRCS=$(wildcard $(SRC)/*.cpp)
OBJS=$(patsubst $(SRC)/%.cpp, $(BIN)/%.o, $(SRCS))
SDLDIR= "C:/Program Files/SDL
INCDIR= -I $(SDLDIR)/SDL2/include/SDL2" -I $(SDLDIR)/SDL2_Image/include/SDL2" -I $(SDLDIR)/SDL2_ttf/include/SDL2"
LIBDIR= -L $(SDLDIR)/SDL2/lib" -L $(SDLDIR)/SDL2_Image/lib" -L $(SDLDIR)/SDL2_ttf/lib"
LIB=-l SDL2main -l SDL2 -l SDL2_image -l SDL2_ttf
EXEC=Program

all:$(EXEC)
	echo All

$(EXEC): $(OBJS)
	echo Linking
	$(CC) $(CFLAGS) $(OBJS) $(LIBDIR) $(LIB) -o $@

$(BIN)/%.o: $(SRC)/%.cpp
	echo Compiling
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

clean:
	echo Cleaning
	rm *.exe $(BIN)/*.o