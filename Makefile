CC = gcc
LIBSPARENT = -lGL -lGLU -lglut 
LIBSLAST = -lglut -lGLU -lGL
LIBSPLAYER = -lm
LDFLAGS = -lglut -lGLU -lGL -lm

all: main liquid_line pills_line manager animation

main: main.o
	$(CC) -o main main.o $(LIBSPARENT) $(LIBSLAST) $(LIBSPLAYER)

liquid_line: liquid_line.o
	$(CC) -o liquid_line liquid_line.o $(LIBSPARENT) $(LIBSLAST) $(LIBSPLAYER)

pills_line: pills_line.o
	$(CC) -o pills_line pills_line.o $(LIBSPARENT) $(LIBSLAST) $(LIBSPLAYER)

manager: manager.o
	$(CC) -o manager manager.o $(LIBSPARENT) $(LIBSLAST) $(LIBSPLAYER)

main.o: main.c
	$(CC) -c -o main.o main.c

liquid_line.o: liquid_line.c
	$(CC) -c -o liquid_line.o liquid_line.c

pills_line.o: pills_line.c
	$(CC) -c -o pills_line.o pills_line.c

manager.o: manager.c
	$(CC) -c -o manager.o manager.c

animation: animation.c
	$(CC) animation.c -o animation $(LDFLAGS)
clean:
	rm -f main.o liquid_line.o pills_line.o manager.o main liquid_line pills_line manager animation

