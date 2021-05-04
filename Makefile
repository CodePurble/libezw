# adapted from https://spin.atomicobject.com/2016/08/26/makefile-c-projects/
TARGET_EXEC ?= main

BUILD_DIR ?= ./build
SRC_DIR ?= ./src
INC_DIR ?= ./include

SRCS := $(shell find $(SRC_DIR) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_FLAGS := $(addprefix -I,$(INC_DIR))

CFLAGS ?= $(INC_FLAGS) -Wall -MMD -MP

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)/*

-include $(DEPS)

MKDIR_P ?= mkdir -p
