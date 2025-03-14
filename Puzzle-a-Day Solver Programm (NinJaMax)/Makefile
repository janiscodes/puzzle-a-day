CC = gcc
SRC_DIR = src
OBJ_DIR = obj
EXEC = puzzle-a-day

# List all the source files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/dfs.c $(SRC_DIR)/pruning_flood_fill.c $(SRC_DIR)/puzzle.c

# Create object file paths by replacing .c with .o
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default target
all: $(EXEC)

# Rule to build the executable
$(EXEC): $(OBJS)
	@$(CC) $(OBJS) -o $(EXEC)

# Rule to compile .c files into .o files in the obj directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)   # Ensure the obj directory exists
	@$(CC) $(CFLAGS) -c $< -o $@

# Run the program after compilation
run: $(EXEC)
	@./$(EXEC)

# Clean up object files and executable
clean:
	@rm -rf $(OBJ_DIR) $(EXEC)
