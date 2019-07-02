CFLAGS=-Wall -g -std=gnu99 # -fsanitize=address
CC=clang

mdp: mdp.c mdp.h
	${CC} ${CFLAGS} -c mdp.c

start: mdp start.c
	${CC} ${CFLAGS} -o start start.c mdp.o

utilities: utilities.c utilities.h
	${CC} ${CFLAGS} -c utilities.c

# NB: value and policy depend on utilities, but the dependency is removed so 
# the calc_eu and calc_meu stubs are not inadvertently compiled

value: mdp value_iteration.c
	${CC} ${CFLAGS} -o  value_iteration value_iteration.c mdp.o utilities.o

policy: mdp policy_iteration.c policy_evaluation.c
	${CC} ${CFLAGS} -c policy_evaluation.c 
	${CC} ${CFLAGS} -o policy_iteration policy_iteration.c  \
	mdp.o utilities.o policy_evaluation.o

environment: mdp
	${CC} ${CFLAGS} -c environment.c


td: mdp environment td.c
	${CC} ${CFLAGS} -o td td.c \
	mdp.o environment.o

max: max.c max.h
	${CC} ${CFLAGS} -c max.c

qlearn: mdp max environment qlearn.c
	${CC} ${CFLAGS} -o qlearn qlearn.c \
	mdp.o environment.o max.o

tidy: 
	rm -f *~

clean: tidy # NB: Does NOT delete utilities.o
	rm -f environment.o max.o mdp.o policy_evaluation.o 
	rm -f value_iteration policy_iteration adp td qlearn

adp: policy environment # Old target for ADP. Not currently used.
	${CC} ${CFLAGS} -o adp adp.c \
	policy_evaluation.o mdp.o environment.o utilities.o
