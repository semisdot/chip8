
CC = gcc

CPPFLAGS = $(CPP_DIRS)
CFLAGS = $(WARN_OPTS)

WARN_OPTS = -Wall -Wextra

LDFLAGS = $(LD_DIRS)
LDLIBS = -lSDL2

CPP_DIRS = -I$(INC_DIR)
# LD_DIRS = -L

# ----------------------------------------------------------------------------------------------------

BIN = chip8

BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)

SRC_DIR = ./src
INC_DIR = ./inc

# ----------------------------------------------------------------------------------------------------

_SRC = $(wildcard $(SRC_DIR)/*.c)
_INC = $(wildcard $(INC_DIR)/*.h)
_OBJ = $(wildcard $(OBJ_DIR)/*.o)

# ----------------------------------------------------------------------------------------------------

# first rule
.PHONY: all
all: build

# ----------------------------------------------------------------------------------------------------

OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(_SRC))

# compiling the program
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(_INC)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# linking the program
$(BIN): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# ----------------------------------------------------------------------------------------------------

.PHONY: build
build: dir bin

.PHONY: dir
dir:
	@mkdir -p -v $(BUILD_DIR)

.PHONY: bin
bin: $(BIN)

.PHONY: run
run: build
	@./$(BIN)

.PHONY: clean
clean:
	@$(RM) -v $(_OBJ)
	@$(RM) -d -v $(BUILD_DIR)
	@$(RM) -v $(BIN)
