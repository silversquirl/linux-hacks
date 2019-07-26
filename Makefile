CC := clang
CFLAGS := -std=c99 -pedantic -Wall -Os
LDFLAGS := -static -s

.PHONY: all
all: cterm drop textmode

%: %.c
	${CC} ${CFLAGS} -o $@ $< ${LDFLAGS}
