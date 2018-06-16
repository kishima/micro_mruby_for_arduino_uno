CFLAGS = -Wall -Wpointer-arith -g -DMMRB_DEBUG -DMMRB_PCENV

COMMON_SRCS = mmruby.c load.c vm.c
RUBY_LIB_SRCS = 
TARGET = libmmruby.a
OBJS = $(COMMON_SRCS:.c=.o) $(RUBY_LIB_SRCS:.c=.o)

all:
	$(MAKE) $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $?

mmruby.o: mmruby.c

vm.o: vm.c

load.o: load.c

clean:
	@rm -Rf $(TARGET) $(OBJS) *~

