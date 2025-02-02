CC=gcc
CFLAGS=-I$(CURDIR)/include -g
SRC_DIR=src
BUILD_DIR=build
TEST_DIR=test

# Main program source files (excluding main.c)
SRCS=$(shell find $(SRC_DIR) -name '*.c' -not -name 'main.c' -not -path '*/\.*')
OBJS=$(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
EXEC=$(BUILD_DIR)/program

# Test files
TEST_SRCS=$(wildcard $(TEST_DIR)/*.c)
TEST_OBJS=$(TEST_SRCS:$(TEST_DIR)/%.c=$(BUILD_DIR)/test/%.o)
TEST_EXEC=$(BUILD_DIR)/test_runner

all: $(BUILD_DIR) $(EXEC)

# Main program
$(EXEC): $(BUILD_DIR)/main.o $(OBJS)
	$(CC) $^ -o $(EXEC)

# Test build
test: $(TEST_EXEC)
	./$(TEST_EXEC)

$(TEST_EXEC): $(OBJS) $(TEST_OBJS)
	$(CC) $^ -lcunit -o $(TEST_EXEC)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/test
	mkdir -p $(BUILD_DIR)/sorting
	mkdir -p ${BUILD_DIR}/searching

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all test clean