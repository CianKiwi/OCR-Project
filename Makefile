
CC=g++
CFLAGS= -g -Wall
SRC=src
BIN=bin
SRCS=$(wildcard $(SRC)/*.cpp)
OBJS=$(patsubst $(SRC)/%.cpp, $(BIN)/%.o, $(SRCS))
SDLDIR= "C:/Users/Cian/Libraries/SDL2/dev libs
INCDIR= -I $(SDLDIR)/SDL2/include" -I $(SDLDIR)/SDL2_Image/include" -I $(SDLDIR)/SDL2_ttf/include"
LIBDIR= -L $(SDLDIR)/SDL2/lib/x64" -L $(SDLDIR)/SDL2_Image/lib/x64" -L $(SDLDIR)/SDL2_ttf/lib/x64"
LIB=-l SDL2 -l SDL2main -l SDL2_image -l SDL2_ttf
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