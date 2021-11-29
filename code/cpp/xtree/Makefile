CXX = g++

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/cpptree
LIB := $(BIN_DIR)/cpptreelib.a
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPPFLAGS := -O3 -Iinclude -MMD -MP
LDFLAGS  := -Llib
LDLIBS   := 

.PHONY: all
all: $(EXE)

$(EXE): $(LIB)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@ 

.PHONY: cpptreelib
treelib: $(LIB)

$(LIB): $(OBJ) | $(BIN_DIR)
	ar rvs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ 

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	@$(RM) -rv $(OBJ_DIR)
	rm $(BIN_DIR)/cpptree $(BIN_DIR)/cpptreelib.a

-include $(OBJ:.o=.d)
