TARGETS = mmruby_test
CFLAGS = -g -I ../src -Wall -Wpointer-arith
LDFLAGS = -L ../src
LIBMMRUBY = ../src/libmmruby.a

all: $(TARGETS)

mmruby_test: main.c $(LIBMMRUBY)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ main.c $(LIBMMRUBY)

clean:
	@rm -f $(TARGETS) *~

