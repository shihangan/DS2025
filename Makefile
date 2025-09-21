# MySTL Makefile

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# 目标文件
TARGETS = test_vector example

# 默认目标
all: $(TARGETS)

# 编译测试程序
test_vector: test_vector.cpp vector.h vector.tpp
	$(CXX) $(CXXFLAGS) test_vector.cpp -o test_vector

# 编译示例程序
example: example.cpp vector.h vector.tpp
	$(CXX) $(CXXFLAGS) example.cpp -o example

# 运行测试
test: test_vector
	./test_vector

# 运行示例
run: example
	./example

# 清理
clean:
	rm -f $(TARGETS) *.exe *.obj

# 调试版本
debug: CXXFLAGS = -std=c++11 -Wall -Wextra -g -DDEBUG
debug: $(TARGETS)

# 帮助信息
help:
	@echo "可用目标:"
	@echo "  all      - 编译所有程序"
	@echo "  test_vector - 编译测试程序"
	@echo "  example  - 编译示例程序"
	@echo "  test     - 运行测试"
	@echo "  run      - 运行示例"
	@echo "  debug    - 编译调试版本"
	@echo "  clean    - 清理生成的文件"
	@echo "  help     - 显示此帮助信息"

.PHONY: all test run clean debug help