CXX = g++
CXXFLAGS = -std=c++14 -O2

SRC_DIR = $(CURDIR)/
BUILD_DIR = build_temp/
SOURCES = $(wildcard $(SRC_DIR)*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)%.cpp,$(BUILD_DIR)%.o,$(SOURCES))
TARGET = concraft

# 默认使用 5 个线程编译
.DEFAULT_GOAL := all
MAKEFLAGS += -j5

all: $(TARGET)
	@echo "程序已编译完成"

# 启动程序
run: $(TARGET)
	$(SRC_DIR)$<

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all run clean
