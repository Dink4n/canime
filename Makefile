.POSIX:

# includes and libs
CFLAGS = $(INCS) -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lcurl

# paths
OBJ_DIR = obj
DEST_DIR = /usr/local

BIN = canime
SRC = src/main.c src/common.c src/web_client.c src/parser.c src/provider.c src/sites.c src/utils.c
OBJ = $(patsubst src/%.c,obj/%.o,$(SRC))

.PHONY: all options clean install uninstall

all: options $(OBJ_DIR) $(BIN)

options:
	@echo canime build options:
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "CC      = $(CC)"

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LDFLAGS)

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	$(RM) -rf $(OBJ_DIR) $(BIN)

install: $(BIN)
	mkdir -p $(DEST_DIR)/bin
	cp -f $(BIN) $(DEST_DIR)/bin
	chmod 755 $(DEST_DIR)/bin/$(BIN)

uninstall:
	rm -f $(DEST_DIR)/bin/$(BIN)
