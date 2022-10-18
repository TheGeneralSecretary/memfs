MAKE_DIR = $(PWD)

SRC_DIR       := $(MAKE_DIR)/src
INCLUDE_DIR   := $(MAKE_DIR)/include
BUILD_DIR	  := $(MAKE_DIR)/build
OBJ_DIR       := $(BUILD_DIR)/obj
LIB_DIR       := $(MAKE_DIR)/lib
BIN_DIR       := $(BUILD_DIR)/bin
EXTERNAL_DIR  := $(MAKE_DIR)/external

# External Libraries DIR
LIBCLOG_DIR   := $(EXTERNAL_DIR)/libclog

INC_SRCH_PATH := 
INC_SRCH_PATH += -I$(SRC_DIR)
INC_SRCH_PATH += -I$(INCLUDE_DIR)
INC_SRCH_PATH += -I$(LIBCLOG_DIR)

LIB_SRCH_PATH :=
LIB_SRCH_PATH += -L$(MAKE_DIR)/lib

CC = gcc
LD = ld

BIN = $(BUILD_DIR)/bin/memfs

LIBS := `pkg-config fuse3 --libs`

CFLAGS := -std=gnu99 -pedantic -Wall -Wextra `pkg-config fuse3 --cflags`
CFLAGS += $(INC_SRCH_PATH) $(LIB_SRCH_PATH)

ifeq ($(BUILD),dev)
# "Dev" build - no optimization, and debugging symbols
CFLAGS += -g -ggdb3 -Og -DDEBUG -finstrument-functions -fdump-rtl-expand -fsanitize=address,undefined -fno-omit-frame-pointer -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
LIBS   += -lclog-dev
else
# "Release" build - optimization, and no debug symbols
CFLAGS += -O2 -s -DNDEBUG
LIBS   += -lclog
endif

LDFLAGS :=

export MAKE_DIR CC LD CFLAGS LDFLAGS LIBS INC_SRCH_PATH OBJ_DIR LIB_DIR BIN_DIR BIN LIBCLOG_DIR

all:
	@$(MAKE) -C $(EXTERNAL_DIR)
	@$(MAKE) -C $(SRC_DIR)

dev:
	make "BUILD=dev"

mount:
	$(BIN) $(ARGS)

valgrind:
	@mkdir -p $(BUILD_DIR)
	valgrind --leak-check=full \
        --show-leak-kinds=all \
        --track-origins=yes \
        --verbose \
        --log-file=$(BUILD_DIR)/valgrind-out.txt \
        $(BIN) $(ARGS)

clangformat:
	find . -iname *.h -o -iname *.c | xargs clang-format -i

.PHONY: clean
clean:
	@$(MAKE) -C $(EXTERNAL_DIR)   clean
	@$(MAKE) -C $(SRC_DIR)		  clean
