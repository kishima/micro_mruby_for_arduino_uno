TARGET ?= TransCoder
SRC_DIRS ?= .

CC=cc

SRCS := $(shell find $(SRC_DIRS) -name '*.c' )
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

LDLIBS =

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

INC_FLAGS += $(INC_FLAGS) -g

CFLAGS := -DMMRUBY_TRANSCODE -g

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS) $(INC_FLAGS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)





