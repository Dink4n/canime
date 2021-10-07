.POSIX:

CFLAGS := -Wall -Wextra -Wpedantic -D_POSIX_C_SOURCE=200809L
LDFLAGS := -lcurl

BIN := canime
SRC := src/main.c src/common.c src/web_client.c src/parser.c src/provider.c src/sites.c src/utils.c
OBJ := $(patsubst src/%.c,obj/%.o,$(SRC))
OBJ_DIR := obj

.PHONY: all clean

all: $(OBJ_DIR) $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LDFLAGS)

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	$(RM) -rf $(OBJ_DIR) $(BIN)
