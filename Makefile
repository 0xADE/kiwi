SRCDIR:=src
SRC = $(wildcard examples/*.c ${SRCDIR}/*.c ${SRCDIR}/*.h)
#OBJ = $(SRC:.c=.o)
OBJ := $(patsubst $(SRCDIR)/%.c,$(SRCDIR)/%.o,$(SRC))
DEP = $(OBJ:.o=.d)  # one dependency file for each source

CFLAGS += -Wall
# generate phony deps during compilation
CFLAGS += -MMD -MP -I${SRCDIR}
#LDFLAGS +=
DEBUG = -g -O0

# rule to generate a dep file by using the C preprocessor
# (man cpp for details on the -MM and -MT options)
%.d: %.c
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

example: ${OBJ}
	@echo " LINK $^"
	$(CC) ${CFLAGS} ${LDFLAGS} -o $@ $^

example-debug: ${OBJ}
	@echo " LINK $^"
	gcc ${DEBUG} ${CFLAGS} ${LDFLAGS} -o $@ $^

clean:
	rm -f example example-debug *.d ${SRC}/*.o

-include $(DEPS)
