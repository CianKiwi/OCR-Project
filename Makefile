
CC=g++
CFLAGS= -g -Wall
SRC=src
BIN=bin
SRCS=$(wildcard $(SRC)/*.cpp)
OBJS=$(patsubst $(SRC)/%.cpp, $(BIN)/%.o, $(SRCS))
INCDIR= -I $(SRC)/includes/SDL2 -I $(SRC)/includes/SDL2_image -I $(SRC)/includes/SDL2_ttf
LIBDIR= -L $(BIN)
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