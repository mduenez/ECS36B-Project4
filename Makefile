# Define the tools
AR=ar
CC=gcc
CXX=g++

# Define the directories
INC_DIR				= ./include
SRC_DIR				= ./src
TESTSRC_DIR			= ./testsrc
BIN_DIR				= ./bin
OBJ_DIR				= ./obj
LIB_DIR				= ./lib
TESTOBJ_DIR			= ./testobj
TESTBIN_DIR			= ./testbin
TESTCOVER_DIR		= ./htmlconv
<<<<<<< HEAD

# Define the flags for compilation/linking
DEFINES				=
INCLUDE				= -I $(INC_DIR) -I /usr/include
ARFLAGS				= rcs
CFLAGS				= -Wall
CPPFLAGS			= -std=c++20
LDFLAGS				= 

TEST_CFLAGS 		= $(CFLAGS) -O0 -g --coverage
TEST_CPPFLAGS 		= $(CPPFLAGS) -fno-inline --coverage
TEST_LDFLAGS		= $(LDFLAGS) -lexpat -lgtest -lgtest_main -lpthread

# Define the object files
SVG_OBJ 			= $(OBJ_DIR)/svg.o
TEST_SVG_OBJ		= $(TESTOBJ_DIR)/svg.o
TEST_SVG_TEST_OBJ	= $(TESTOBJ_DIR)/SVGTest.o
TEST_OBJ_FILES		= $(TEST_SVG_OBJ) $(TEST_SVG_TEST_OBJ)
XML_OBJ              = $(OBJ_DIR)/XMLReader.o
XMLBS_OBJ            = $(OBJ_DIR)/XMLBusSystem.o
OSM_OBJ              = $(OBJ_DIR)/OpenStreetMap.o

TEST_XML_OBJ         = $(TESTOBJ_DIR)/XMLReader.o
TEST_XML_TEST_OBJ    = $(TESTOBJ_DIR)/XMLReaderTest.o

TEST_XMLBS_OBJ       = $(TESTOBJ_DIR)/XMLBusSystem.o
TEST_XMLBS_TEST_OBJ  = $(TESTOBJ_DIR)/XMLBusSystemTest.o

TEST_OSM_OBJ         = $(TESTOBJ_DIR)/OpenStreetMap.o
TEST_OSM_TEST_OBJ    = $(TESTOBJ_DIR)/OpenStreetMapTest.o

STRSRC_OBJ            = $(OBJ_DIR)/StringDataSource.o
TEST_STRSRC_OBJ       = $(TESTOBJ_DIR)/StringDataSource.o
# Define the targets
TEST_TARGET					= $(TESTBIN_DIR)/testsvg
TEST_XML_TARGET      		= $(TESTBIN_DIR)/testxml
TEST_XMLBS_TARGET    		= $(TESTBIN_DIR)/testxmlbs
TEST_OSM_TARGET      		= $(TESTBIN_DIR)/testosm
LIBSVG						= $(LIB_DIR)/libsvg.a

all: directories $(LIBSVG) runtests

$(OBJ_DIR)/svg.o: $(SRC_DIR)/svg.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(LIBSVG): $(SVG_OBJ)
	$(AR) $(ARFLAGS) $@ $^

runtests: $(TEST_XML_TARGET) \
          $(TEST_XMLBS_TARGET) \
          $(TEST_OSM_TARGET)

	./$(TEST_XML_TARGET)
	./$(TEST_XMLBS_TARGET)
	./$(TEST_OSM_TARGET)

	lcov --capture --directory . --output-file $(TESTCOVER_DIR)/coverage.info --ignore-errors inconsistent,source,mismatch
	lcov --remove $(TESTCOVER_DIR)/coverage.info '/usr/*' '*/testsrc/*' --output-file $(TESTCOVER_DIR)/coverage.info
	genhtml $(TESTCOVER_DIR)/coverage.info --output-directory htmlcov

$(TEST_TARGET): $(TEST_OBJ_FILES)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(TEST_OBJ_FILES) $(TEST_LDFLAGS) -o $(TEST_TARGET)

$(TEST_SVG_OBJ): $(SRC_DIR)/svg.c
	$(CC) $(TEST_CFLAGS) $(DEFINES) $(INCLUDE) -c $(SRC_DIR)/svg.c -o $(TEST_SVG_OBJ)

<<<<<<< HEAD
$(TEST_SVG_TEST_OBJ): $(TESTSRC_DIR)/SVGTest.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(DEFINES) $(INCLUDE) -c $(TESTSRC_DIR)/SVGTest.cpp -o $(TEST_SVG_TEST_OBJ)

$(OBJ_DIR)/XMLBusSystem.o: $(SRC_DIR)/XMLBusSystem.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TESTOBJ_DIR)/XMLBusSystem.o: $(SRC_DIR)/XMLBusSystem.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TESTOBJ_DIR)/XMLBusSystemTest.o: $(TESTSRC_DIR)/XMLBusSystemTest.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TEST_XMLBS_TARGET): $(TEST_XMLBS_OBJ) $(TEST_XMLBS_TEST_OBJ) $(TEST_XML_OBJ)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $^ $(TEST_LDFLAGS) -o $@

$(OBJ_DIR)/OpenStreetMap.o: $(SRC_DIR)/OpenStreetMap.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TESTOBJ_DIR)/OpenStreetMap.o: $(SRC_DIR)/OpenStreetMap.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TESTOBJ_DIR)/OpenStreetMapTest.o: $(TESTSRC_DIR)/OpenStreetMapTest.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TEST_OSM_TARGET): $(TEST_OSM_OBJ) $(TEST_OSM_TEST_OBJ) $(TEST_XML_OBJ)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $^ $(TEST_LDFLAGS) -o $@

$(OBJ_DIR)/XMLReader.o: $(SRC_DIR)/XMLReader.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TESTOBJ_DIR)/XMLReader.o: $(SRC_DIR)/XMLReader.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TESTOBJ_DIR)/XMLReaderTest.o: $(TESTSRC_DIR)/XMLReaderTest.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/StringDataSource.o: $(SRC_DIR)/StringDataSource.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TESTOBJ_DIR)/StringDataSource.o: $(SRC_DIR)/StringDataSource.cpp
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TEST_XML_TARGET): $(TEST_XML_OBJ) $(TEST_XML_TEST_OBJ) $(TEST_STRSRC_OBJ)
	$(CXX) $(TEST_CFLAGS) $(TEST_CPPFLAGS) $^ $(TEST_LDFLAGS) -o $@

directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(TESTOBJ_DIR)
	mkdir -p $(TESTBIN_DIR)
	mkdir -p $(TESTCOVER_DIR)

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB_DIR)
	rm -rf $(TESTOBJ_DIR)
	rm -rf $(TESTBIN_DIR)
	rm -rf $(TESTCOVER_DIR)

