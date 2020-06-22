all: /*
	if ! test -d "bin" ; then mkdir bin ; fi
	gcc src/*.c src/*/*.c `pkg-config vips --cflags --libs` -o bin/main && ./bin/main -?