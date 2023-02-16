
C := ../c_realisation
DDLOG := ../ddlog_realisation
DDLOG_LIB := $(DDLOG)/reachability_ddlog/target/release/libreachability_ddlog.a

ALL: main

file_op.o:
	gcc -c $(C)/file_op.c

graph_op.o:
	gcc -c $(C)/graph_op.c

matrix_op.o:
	gcc -c $(C)/matrix_op.c

libc_realisation.a: file_op.o graph_op.o matrix_op.o
	ar cr libc_realisation.a graph_op.o file_op.o matrix_op.o

ddlog_util.o:
	gcc -c $(DDLOG)/ddlog_util.c -I $(DDLOG)/reachability_ddlog -lpthread -ldl -lm

experiment.o: experiment.c
	gcc -c experiment.c -I ../ddlog_realisation/reachability_ddlog/

main: experiment.o ddlog_util.o libc_realisation.a
	gcc -o main experiment.o \
	ddlog_util.o \
	$(DDLOG_LIB) \
	libc_realisation.a \
	-lpthread -ldl -lm

clean:
	rm -f *.o *.a main