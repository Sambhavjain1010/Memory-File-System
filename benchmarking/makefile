# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Executable names
MAIN_EXEC = memFS
USER_MODE_EXEC = user_mode
BENCHMARK_MODE_EXEC = benchmark_mode

# Source files
MAIN_SRC = memFSmain.c
USER_MODE_SRC = user_mode.c
BENCHMARK_MODE_SRC = benchmark_mode.c

# Build all executables and give execute permissions
all: $(MAIN_EXEC) $(USER_MODE_EXEC) $(BENCHMARK_MODE_EXEC)
	chmod +x $(MAIN_EXEC) $(USER_MODE_EXEC) $(BENCHMARK_MODE_EXEC)

# Rule to build main program
$(MAIN_EXEC): $(MAIN_SRC)
	$(CC) $(CFLAGS) -o $(MAIN_EXEC) $(MAIN_SRC)

# Rule to build user mode program
$(USER_MODE_EXEC): $(USER_MODE_SRC)
	$(CC) $(CFLAGS) -o $(USER_MODE_EXEC) $(USER_MODE_SRC)

# Rule to build benchmark mode program
$(BENCHMARK_MODE_EXEC): $(BENCHMARK_MODE_SRC)
	$(CC) $(CFLAGS) -o $(BENCHMARK_MODE_EXEC) $(BENCHMARK_MODE_SRC)

# Clean up all executables
clean:
	rm -f $(MAIN_EXEC) $(USER_MODE_EXEC) $(BENCHMARK_MODE_EXEC)
