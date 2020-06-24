COMPILER = gcc
BUILD_DIR = ${CURDIR}/bin
CFLAGS = -Wall `pkg-config vips --cflags --libs`

.SILENT: all clean

all: /*
	if ! test -d ${BUILD_DIR} ; then mkdir ${BUILD_DIR} ; fi
	${COMPILER} src/main.c src/*/*.c ${CFLAGS} -o ${BUILD_DIR}/main && ${BUILD_DIR}/main -?

clean: 
	rm -rf ${BUILD_DIR}