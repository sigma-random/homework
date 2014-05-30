OBJS   = sysv_shm.o 
BIN = sysv_shm
CFLAGS = -lpthread
main: ${OBJS}
	gcc   ${OBJS}  ${CFLAGS}  -o ${BIN}

clean:
	rm ${OBJS} ${BIN}
