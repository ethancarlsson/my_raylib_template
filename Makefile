CC=cc
BUILDDIR=desktop_build
BIN=game

INC=-Idesktop_inc

PKG_CONF=`pkg-config --libs --cflags raylib`

FLAGS=-c

SRC=src/game.c src/main.c 
OBJ=src/game.o src/main.o


WEB_CC=emcc
WEB_DIR=web_build
WEB_BIN=$(WEB_DIR)/main.html
WEB_FLAGS=-Iinclude
WEB_LDFLAGS=-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1 -Iinclude/ -Llib/
WEB_LIBS=-lraylib
WEB_OBJ=src/game_w.o src/main_w.o


all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) $(PKG_CONF) -o $(BIN)

%.o:%.c
	$(CC) -c -o $@ $^ $(INC)


clean:
	rm src/*.o; rm $(BIN); rm -r $(WEB_BIN) $(WEB_DIR)/*.wasm $(WEB_DIR)/*.js


web: $(WEB_BIN)

$(WEB_BIN): $(WEB_OBJ)
	$(WEB_CC) $(WEB_OBJ) $(WEB_LDFLAGS) $(WEB_LIBS) -o $(WEB_BIN)

%_w.o:%.c
	$(WEB_CC) $(WEB_FLAGS) -c $< -o $@
