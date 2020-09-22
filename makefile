PROG=jsonParser
all=$(PROG)
CFLAGS=-Wall -Werror -Wextra -g

$(PROG): *.c *.h
	gcc $(CFLAGS) -o $(PROG) *.c *.h

.PHONY:clean
clean:
	rm -f $(PROG)
