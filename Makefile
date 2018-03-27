LIB_SRC=$(wildcard src/*.c)
LIB_OBJ=$(LIB_SRC:.c=.o)

TESTS_SRC=$(wildcard test/*.c)
TESTS_EXES=$(TESTS_SRC:.c=.test)

CFLAGS += -std=c99 -Iinclude -DDEBUG
LDFLAGS +=

all: libfarmbot.so.0

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

libfarmbot.so.0.0.1: $(LIB_OBJ)
	$(CC) -shared -Wl,-soname,libfarmbot.so.0 -o libfarmbot.so.0.0.1 $(LIB_OBJ)

libfarmbot.so.0: libfarmbot.so.0.0.1
	$(RM) $@
	ln -s $< $@

test: all $(TESTS_EXES)

%.test: %.c
	$(CC) $(CFLAGS) -Wl,-R -Wl,./ libfarmbot.so.0 -o $@ $<
	$@

clean-test:
	$(RM) $(TESTS_EXES)

clean-lib:
	$(RM) $(LIB_OBJ)
	$(RM) libfarmbot.so.0.0.1 libfarmbot.so.0

clean: clean-lib clean-test
