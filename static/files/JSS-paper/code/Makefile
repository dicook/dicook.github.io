COMPILER = cc
DEBUG=-g
DEBUG=
LD=-Bstatic
LD        = -non_shared
LD=
CFLAGS=${DEBUG} ${OPT}
IDIR= -I/usr/include
LDIR= -L/usr/lib
DNET_STUB = -ldnet_stub
LIBS= ${LDIR} -lm ${DNET_STUB}

.KEEP_STATE:

COBJ=read.c gt.c util.c gt_util.c svd.c

OBJ=read.o gt.o util.o gt_util.o svd.o

gt: main.o ${OBJ}
	${COMPILER} ${CFLAGS} ${LD} -o wombat main.o ${OBJ} ${LIBS}

main.o:
	${COMPILER} -c main.c ${CFLAGS} ${IDIR}

.c.o:
	${COMPILER} -c $*.c ${CFLAGS} ${IDIR}


