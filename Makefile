TARGET = iso_game

SRC_DIR = .
GLAD_DIR = glad
INC_DIR = include
OBJ_DIR = obj

PROJECT_SRCS = $(filter-out $(GLAD_DIR)/glad.c, $(wildcard $(SRC_DIR)/*.cpp))
PROJECT_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(PROJECT_SRCS))

GLAD_SRC = $(GLAD_DIR)/glad.c
GLAD_OBJ = $(GLAD_DIR)/glad.o

CXX = g++
CXXFLAGS = -std=c++17 -Wall -I$(INC_DIR)
LDFLAGS = -lglfw -lGL -ldl -lX11 -lpthread -lXrandr -lXi

# Debug
$(info PROJECT_SRCS = $(PROJECT_SRCS))
$(info PROJECT_OBJS = $(PROJECT_OBJS))
$(info GLAD_OBJ = $(GLAD_OBJ))

all: $(TARGET)

$(TARGET): $(PROJECT_OBJS) $(GLAD_OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(GLAD_OBJ): $(GLAD_SRC)
	$(CXX) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(GLAD_OBJ) $(TARGET)

.PHONY: all clean
