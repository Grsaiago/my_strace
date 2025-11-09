CC = clang
CFLAGS = -g \
-Wall \
-Wextra \
-Werror \
-Wpedantic \
-I./include

NAME = my_strace
TEST_NAME = test_suite

SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = tests

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TEST_OBJS = $(filter-out $(OBJ_DIR)/main.o, $(OBJS))
TESTS = $(wildcard $(TEST_DIR)/*.c)

.PHONY: all
all: help

.PHONY: help
help: ## Prints help for targets with comments
	@echo "Available Rules:"
	@cat $(MAKEFILE_LIST) | grep -E '^[a-zA-Z_-]+:.*?## .*$$' | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

.PHONY: build
build: $(OBJ_DIR) $(NAME) ## Builds the executable

.PHONY: clean
clean: ## Deletes transitive dependencies
	@rm -f $(OBJS)

.PHONY: fclean
fclean: clean ## Deletes transitive dependencies + the executable
	@rm -f $(NAME)

$(OBJ_DIR):
	@mkdir -p obj

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $(CFLAGS) $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(TEST_NAME): $(TESTS) $(TEST_OBJS)
	$(CC) -g $(TEST_OBJS) $(TESTS) -lcriterion -Iinclude -o $(TEST_NAME)

.PHONY: test
test: $(TEST_NAME) ## Builds and runst the test suite
	@./$(TEST_NAME)
