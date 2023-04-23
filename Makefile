CC=cc
BUILDDIR=desktop_build
BIN=villages

INC=-Idesktop_inc

PKG_CONF=`pkg-config --libs --cflags raylib`

FLAGS=-c

SRC=src/game.c src/main.c 
OBJ=src/game.o src/main.o 

#
# cd desktop_build && cc -c ../src/game.c ../src/main.c -I../desktop_inc; cc game.o main.o `pkg-config --libs --cflags raylib` -o villages && cd ..;

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) $(PKG_CONF) -o $(BIN)

%.o:%.c
	$(CC) -c -o $@ $^ $(INC)


clean:
	rm src/*.o; rm src/$(BIN);

