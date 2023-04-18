ifeq(($PLATFORM),"web") 
	CC=emcc
	CFLAGS=-Iinclude/
	LDFLAGS=-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1 -Iinclude/ -Llib/
	LIBS=-lraylib
	OBJS=raygame.o
	BIN=main.html

	all:$(BIN)

	main.html: $(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o main.html

	raygame.o: raygame.c
		$(CC) $(CFLAGS) -c $< -o $@

	clean:
		$(RM) -r main.html *.wasm *.o *.dSYM
else
	CC=cc
	CFLAGS=-Iinclude/
	LDFLAGS=-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1 -Iinclude/ -Llib/
	LIBS=-lraylib
	OBJS=raygame.o
	BIN=main.html

	all:$(BIN)

	main.html: $(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o main.html

	raygame.o: raygame.c
		$(CC) $(CFLAGS) -c $< -o $@

	clean:
		$(RM) -r main.html *.wasm *.o *.dSYM
endif
