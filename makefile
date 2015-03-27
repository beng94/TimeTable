all:
	gcc -g main.c backtrack.c backtrack.h io.c io.h time.h data.c data.h -o ALL `pkg-config --cflags --libs glib-2.0`

backtrack:
	gcc backtrack.c -Wall -o BACKTRACK_C `pkg-config --cflags --libs glib-2.0`

io:
	gcc io.h io.c -Wall -o IO

data:
	gcc data.c data.h -Wall -o DATA `pkg-config --cflags --libs glib-2.0`

time:
	gcc time.h -Wall -o TIME

main:
	gcc main.c -Wall -o MAIN `pkg-config --cflags --libs glib-2.0`
