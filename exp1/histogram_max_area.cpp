#include <iostream>
#include <random>
#include <algorithm>
#include <iomanip>
#include "../vector.h"
#include "../stack.h"

// 计算柱状图中能够勾勒出的矩形的最大面积
int largestRectangleArea(const MySTL::Vector<int>& heights) {
    int n = heights.size();
    if (n == 0) return 0;
    
    MySTL::Stack<int> st; // 用于存储柱子的索引
    int maxArea = 0;
    
    for (int i = 0; i <= n; i++) {
        // 当到达数组末尾或当前柱子高度小于栈顶柱子高度时，计算面积
        int h = (i == n) ? 0 : heights[i];
        
        while (!st.empty() && h < heights[st.top()]) {
            int height = heights[st.top()];
            st.pop();
            
            // 计算宽度：如果栈为空，则宽度为i；否则宽度为i - st.top() - 1
            int width = st.empty() ? i : i - st.top() - 1;
            
            // 更新最大面积
            maxArea = std::max(maxArea, height * width);
        }
        
        st.push(i);
    }
    
    return maxArea;
}

// 生成随机测试数据
MySTL::Vector<MySTL::Vector<int>> generateTestData(int numTests, int maxSize, int maxHeight) {
    MySTL::Vector<MySTL::Vector<int>> testData;
    std::random_device rd;
    std::mt19937 gen(rd());
    
    for (int test = 0; test < numTests; test++) {
        // 随机生成数组大小 (1 <= size <= maxSize)
        std::uniform_int_distribution<> sizeDist(1, maxSize);
        int size = sizeDist(gen);
        
        std::vector<int> heights;
        // 随机生成高度 (0 <= height <= maxHeight)
        std::uniform_int_distribution<> heightDist(0, maxHeight);
        
        for (int i = 0; i < size; i++) {
            heights.push_back(heightDist(gen));
        }
        
        testData.push_back(heights);
    }
    
    return testData;
}

// 打印柱状图（可视化）
void printHistogram(const std::vector<int>& heights) {
    int maxHeight = *std::max_element(heights.begin(), heights.end());
    
    // 打印柱状图
    for (int h = maxHeight; h > 0; h--) {
        for (int i = 0; i < heights.size(); i++) {
            if (heights[i] >= h) {
                std::cout << "█ ";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
    
    // 打印底部数字
    for (int i = 0; i < heights.size(); i++) {
        std::cout << heights[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    // 生成10组测试数据
    int numTests = 10;
    int maxSize = 20;  // 为了可视化效果，限制大小
    int maxHeight = 10; // 为了可视化效果，限制高度
    
    std::vector<std::vector<int>> testData = generateTestData(numTests, maxSize, maxHeight);
    
    std::cout << "柱状图中矩形的最大面积测试：" << std::endl;
    std::cout << "==============================" << std::endl;
    
    for (int i = 0; i < numTests; i++) {
        std::cout << "测试 #" << (i + 1) << ":" << std::endl;
        std::cout << "柱状图高度: ";
        for (int h : testData[i]) {
            std::cout << h << " ";
        }
        std::cout << std::endl;
        
        // 计算最大面积
        int maxArea = largestRectangleArea(testData[i]);
        
        // 打印柱状图可视化
        std::cout << "柱状图可视化:" << std::endl;
        printHistogram(testData[i]);
        
        std::cout << "最大矩形面积: " << maxArea << std::endl;
        std::cout << "==============================" << std::endl;
    }
    
    // 额外测试一些特殊情况
    std::vector<std::vector<int>> specialCases = {
        {2, 1, 5, 6, 2, 3},  // 经典测试用例，最大面积为10
        {2, 4},              // 简单情况
        {0, 0, 0, 0},        // 全零情况
        {}                   // 空数组
    };
    
    std::cout << "特殊测试用例：" << std::endl;
    std::cout << "==============================" << std::endl;
    
    for (int i = 0; i < specialCases.size(); i++) {
        std::cout << "特殊用例 #" << (i + 1) << ":" << std::endl;
        std::cout << "柱状图高度: ";
        for (int h : specialCases[i]) {
            std::cout << h << " ";
        }
        std::cout << std::endl;
        
        // 计算最大面积
        int maxArea = largestRectangleArea(specialCases[i]);
        
        // 打印柱状图可视化（如果不是空数组）
        if (!specialCases[i].empty()) {
            std::cout << "柱状图可视化:" << std::endl;
            printHistogram(specialCases[i]);
        }
        
        std::cout << "最大矩形面积: " << maxArea << std::endl;
        std::cout << "==============================" << std::endl;
    }
    
    return 0;
}