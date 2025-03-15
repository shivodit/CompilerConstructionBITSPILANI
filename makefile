# Group No: 15
# Group members:
# 1. Shivodit Raj Vishnoi - 2022A7PS1221P
# 2. Parsewar Omkar Balaji - 2022A7PS0089P
# 3. Samiksha Kaul - 2022A7PS1169P
# 4. Sohan Reddy Jalakanti - 2022A7PS1177P
# 5. Arnav Gupta - 2022A7PS1189P
# 6. Akshat Gosain - 2022A7PS0154G

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g -O2

# Libraries
LDLIBS = -lm

# Source files
SRCS = driver.c lexer.c helper_function.c symbol_table.c parser.c stack.c parseTree.c

# Header files
HDRS = lexer.h lexerDef.h helper_function.h symbol_table.h symbol_tableDef.h \
       parser.h parserDef.h stack.h stackDef.h parseTree.h parseTreeDef.h

# Object files
OBJS = $(SRCS:.c=.o)

# Executable
EXEC = compiler

# Default target
all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS) $(LDLIBS)

# Compile source files into object files
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(EXEC)

# Clean up all generated files
distclean: clean
	rm -f *~ *.bak

# Run the executable
run: $(EXEC)
	./$(EXEC) $(f1) $(f2)

.PHONY: all clean distclean run
