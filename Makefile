# =======================
# Tools
# =======================
CXX      := g++
AR       := ar
ARFLAGS  := rcs

# =======================
# Directories
# =======================
INC_DIR       := ./include
SRC_DIR       := ./src
TESTSRC_DIR   := ./testsrc
BIN_DIR       := ./bin
OBJ_DIR       := ./obj
LIB_DIR       := ./lib
TESTOBJ_DIR   := ./testobj
TESTBIN_DIR   := ./testbin
TESTCOVER_DIR := ./htmlconv

# =======================
# Compilation Flags
# =======================
CFLAGS      := -Wall
CPPFLAGS    := -std=c++20
LDFLAGS     := -lexpat

TEST_CFLAGS    := $(CFLAGS) -O0 -g --coverage
TEST_CPPFLAGS  := $(CPPFLAGS) -fno-inline --coverage
TEST_LDFLAGS   := /home/mduenez/googletest/build/lib/libgtest_main.a \
                   /home/mduenez/googletest/build/lib/libgtest.a \
                   -L$(LIB_DIR) -lsvg -lpthread -lexpat

INCLUDE := -I $(INC_DIR) -I /usr/include -I /home/mduenez/googletest/googletest/include

# =======================
# Source & Object Files
# =======================
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(filter %.cpp,$(SRC_FILES))) \
             $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(filter %.c,$(SRC_FILES)))

MAIN_SRC := $(SRC_DIR)/main.c
MAIN_OBJ := $(OBJ_DIR)/main.o

LIBSVG   := $(LIB_DIR)/libsvg.a

TEST_SRC_FILES  := $(wildcard $(TESTSRC_DIR)/*.cpp)
TEST_OBJ_FILES  := $(patsubst $(TESTSRC_DIR)/%.cpp,$(TESTOBJ_DIR)/%.o,$(TEST_SRC_FILES))

# Ordered test binaries
TEST_BIN_FILES_ORDERED := \
	$(TESTBIN_DIR)/testsvg \
	$(TESTBIN_DIR)/teststrdatasource \
	$(TESTBIN_DIR)/teststrdatasink \
	$(TESTBIN_DIR)/testxml \
	$(TESTBIN_DIR)/testsvgwriter \
	$(TESTBIN_DIR)/testxmlbussystem \
	$(TESTBIN_DIR)/testosm \
	$(TESTBIN_DIR)/testfiledatass \
	$(TESTBIN_DIR)/testbussystemindexer \
	$(TESTBIN_DIR)/testgeographicutils \
	$(TESTBIN_DIR)/teststreetmapindexer \
	$(TESTBIN_DIR)/testtripplanner \
	$(TESTBIN_DIR)/testtexttripplanwriter \
	$(TESTBIN_DIR)/testsvgtripplanwriter \
	$(TESTBIN_DIR)/testhtmltripplanwriter \
	$(TESTBIN_DIR)/testtripplannercommandline

# =======================
# Targets
# =======================
all: directories $(LIBSVG) tripplanner runtests

# -----------------------
# Object compilation
# -----------------------
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

$(MAIN_OBJ): $(MAIN_SRC)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

# -----------------------
# Library
# -----------------------
$(OBJ_DIR)/svg.o: $(SRC_DIR)/svg.c
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

$(LIBSVG): $(OBJ_DIR)/svg.o
	$(AR) $(ARFLAGS) $@ $^

# -----------------------
# Tripplanner executable
# -----------------------
tripplanner: $(OBJ_FILES) $(MAIN_OBJ) $(LIBSVG)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(OBJ_FILES) $(MAIN_OBJ) -L$(LIB_DIR) -lsvg -lexpat $(LDFLAGS) -o $(BIN_DIR)/tripplanner

# -----------------------
# Test compilation
# -----------------------
$(TESTOBJ_DIR)/%.o: $(TESTSRC_DIR)/%.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TESTOBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TESTOBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(INCLUDE) -c $< -o $@

# -----------------------
# Test executables
# -----------------------
$(TESTBIN_DIR)/%: $(TESTOBJ_DIR)/%.o $(TEST_OBJ_FILES) $(OBJ_FILES) $(LIBSVG)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $^ $(TEST_LDFLAGS) -o $@

# -----------------------
# Run all tests
# -----------------------
runtests: directories $(TEST_BIN_FILES_ORDERED)
	@echo "Running all tests in order..."
	@for test_exec in $(TEST_BIN_FILES_ORDERED); do \
		echo "Running $$test_exec..."; \
		$$test_exec; \
	done
	@echo "Tests complete."

# -----------------------
# Coverage
# -----------------------
coverage: runtests
	@echo "Capturing coverage..."
	lcov --capture --directory . --output-file $(TESTCOVER_DIR)/coverage.info --ignore-errors inconsistent,source,mismatch
	lcov --remove $(TESTCOVER_DIR)/coverage.info '/usr/*' '*/testsrc/*' --output-file $(TESTCOVER_DIR)/coverage.info
	@echo "Generating HTML report..."
	genhtml $(TESTCOVER_DIR)/coverage.info --output-directory $(TESTCOVER_DIR)
	@echo "Coverage report is in $(TESTCOVER_DIR)/index.html"

opencoverage:
	xdg-open $(TESTCOVER_DIR)/index.html || echo "Open htmlconv/index.html manually"

# -----------------------
# Directories
# -----------------------
directories:
	mkdir -p $(BIN_DIR) $(OBJ_DIR) $(LIB_DIR) $(TESTOBJ_DIR) $(TESTBIN_DIR) $(TESTCOVER_DIR)

# -----------------------
# Clean
# -----------------------
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR) $(LIB_DIR) $(TESTOBJ_DIR) $(TESTBIN_DIR) $(TESTCOVER_DIR)
