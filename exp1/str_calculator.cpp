#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>
#include <cctype>
#include <map>
#include <algorithm>
#include "../vector.h"
#include "../stack.h"



// 函数类型定义
enum class FunctionType {
    NONE,
    UNARY,  // 一元函数，如sin, cos, log
    BINARY  // 二元函数，如pow, max, min
};

// 函数信息结构体
struct FunctionInfo {
    FunctionType type;
    std::function<double(double)> unaryFunc;
    std::function<double(double, double)> binaryFunc;
};

// 运算符优先级定义
int operatorPriority(char op) {
    switch (op) {
        case '+': 
        case '-': 
            return 1;
        case '*': 
        case '/': 
            return 2;
        case '^': 
            return 3;
        case '(': 
            return 0;  // 左括号优先级最低
        default: 
            return -1; // 非法运算符
    }
}

// 执行二元运算
double performOperation(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) {
                throw std::runtime_error("除数不能为零");
            }
            return a / b;
        case '^': return std::pow(a, b);
        default: 
            throw std::runtime_error("不支持的运算符");
    }
}

// 字符串计算器类
class StringCalculator {
private:
    MySTL::Stack<double> numStack;    // 数值栈
    MySTL::Stack<char> opStack;       // 运算符栈
    
    // 函数映射表
    std::map<std::string, FunctionInfo> functions;

    // 初始化函数映射表
    void initFunctions() {
        // 一元函数
        functions["sin"] = {FunctionType::UNARY, [](double x) { return std::sin(x); }, nullptr};
        functions["cos"] = {FunctionType::UNARY, [](double x) { return std::cos(x); }, nullptr};
        functions["tan"] = {FunctionType::UNARY, [](double x) { return std::tan(x); }, nullptr};
        functions["asin"] = {FunctionType::UNARY, [](double x) { return std::asin(x); }, nullptr};
        functions["acos"] = {FunctionType::UNARY, [](double x) { return std::acos(x); }, nullptr};
        functions["atan"] = {FunctionType::UNARY, [](double x) { return std::atan(x); }, nullptr};
        functions["sqrt"] = {FunctionType::UNARY, [](double x) { 
            if (x < 0) throw std::runtime_error("负数不能开平方根");
            return std::sqrt(x); 
        }, nullptr};
        functions["log"] = {FunctionType::UNARY, [](double x) { 
            if (x <= 0) throw std::runtime_error("对数函数的参数必须为正数");
            return std::log(x); 
        }, nullptr};
        functions["log10"] = {FunctionType::UNARY, [](double x) { 
            if (x <= 0) throw std::runtime_error("对数函数的参数必须为正数");
            return std::log10(x); 
        }, nullptr};
        functions["exp"] = {FunctionType::UNARY, [](double x) { return std::exp(x); }, nullptr};
        functions["abs"] = {FunctionType::UNARY, [](double x) { return std::abs(x); }, nullptr};
        functions["ceil"] = {FunctionType::UNARY, [](double x) { return std::ceil(x); }, nullptr};
        functions["floor"] = {FunctionType::UNARY, [](double x) { return std::floor(x); }, nullptr};
        functions["round"] = {FunctionType::UNARY, [](double x) { return std::round(x); }, nullptr};
        
        // 二元函数
        functions["max"] = {FunctionType::BINARY, nullptr, [](double a, double b) { return std::max(a, b); }};
        functions["min"] = {FunctionType::BINARY, nullptr, [](double a, double b) { return std::min(a, b); }};
        functions["pow"] = {FunctionType::BINARY, nullptr, [](double a, double b) { return std::pow(a, b); }};
    }

    // 处理数字
    void processNumber(const std::string& expr, size_t& i) {
        double num = 0;
        bool hasDecimal = false;
        double decimalPlace = 0.1;

        // 处理整数部分
        while (i < expr.length() && (std::isdigit(expr[i]) || expr[i] == '.')) {
            if (expr[i] == '.') {
                hasDecimal = true;
                i++;
                continue;
            }

            if (!hasDecimal) {
                num = num * 10 + (expr[i] - '0');
            } else {
                num = num + (expr[i] - '0') * decimalPlace;
                decimalPlace *= 0.1;
            }
            i++;
        }
        i--; // 回退一个位置，因为外层循环会自增
        numStack.push(num);
    }

    // 处理运算符
    void processOperator(char op) {
        if (opStack.empty() || op == '(' || operatorPriority(op) > operatorPriority(opStack.top())) {
            opStack.push(op);
        } else {
            while (!opStack.empty() && opStack.top() != '(' && 
                   operatorPriority(op) <= operatorPriority(opStack.top())) {
                calculateTop();
            }
            opStack.push(op);
        }
    }

    // 处理右括号
    void processRightParenthesis() {
        while (!opStack.empty() && opStack.top() != '(') {
            calculateTop();
        }
        
        if (!opStack.empty() && opStack.top() == '(') {
            opStack.pop(); // 弹出左括号
        } else {
            throw std::runtime_error("括号不匹配");
        }
    }

    // 计算栈顶的一次运算
    void calculateTop() {
        if (numStack.size() < 2 || opStack.empty()) {
            throw std::runtime_error("表达式无效");
        }
        
        double b = numStack.pop();
        double a = numStack.pop();
        char op = opStack.pop();
        
        double result = performOperation(a, b, op);
        numStack.push(result);
    }
    
    // 处理函数
    bool processFunction(const std::string& expr, size_t& i) {
        // 保存当前位置，用于回溯
        size_t start = i;
        
        // 读取可能的函数名
        std::string funcName;
        while (i < expr.length() && std::isalpha(expr[i])) {
            funcName += expr[i++];
        }
        
        // 检查是否是已知函数
        if (functions.find(funcName) != functions.end()) {
            // 跳过空白字符
            while (i < expr.length() && std::isspace(expr[i])) {
                i++;
            }
            
            // 检查是否有左括号
            if (i < expr.length() && expr[i] == '(') {
                i++; // 跳过左括号
                
                // 处理函数参数
                if (functions[funcName].type == FunctionType::UNARY) {
                    // 递归计算参数表达式
                    double arg = calculateSubExpression(expr, i);
                    
                    // 应用一元函数
                    double result = functions[funcName].unaryFunc(arg);
                    numStack.push(result);
                    
                    return true;
                } else if (functions[funcName].type == FunctionType::BINARY) {
                    // 递归计算第一个参数
                    double arg1 = calculateSubExpression(expr, i);
                    
                    // 检查是否有逗号
                    if (i < expr.length() && expr[i] == ',') {
                        i++; // 跳过逗号
                        
                        // 递归计算第二个参数
                        double arg2 = calculateSubExpression(expr, i);
                        
                        // 应用二元函数
                        double result = functions[funcName].binaryFunc(arg1, arg2);
                        numStack.push(result);
                        
                        return true;
                    } else {
                        throw std::runtime_error("二元函数需要两个参数");
                    }
                }
            }
        }
        
        // 不是函数，回溯
        i = start;
        return false;
    }
    
    // 计算子表达式
    double calculateSubExpression(const std::string& expr, size_t& i) {
        // 创建临时计算器来计算子表达式
        StringCalculator subCalculator;
        subCalculator.functions = this->functions; // 复制函数表
        
        // 保存子表达式
        std::string subExpr;
        int parenCount = 1; // 已经遇到一个左括号
        
        while (i < expr.length() && parenCount > 0) {
            if (expr[i] == '(') {
                parenCount++;
            } else if (expr[i] == ')') {
                parenCount--;
                if (parenCount == 0) {
                    i++; // 跳过右括号
                    break;
                }
            } else if (expr[i] == ',' && parenCount == 1) {
                // 遇到同级别的逗号，结束当前参数
                i++; // 跳过逗号
                break;
            }
            
            if (parenCount > 0) {
                subExpr += expr[i++];
            }
        }
        
        // 计算子表达式
        return subCalculator.calculate(subExpr);
    }

public:
    // 构造函数
    StringCalculator() {
        initFunctions();
    }
    
    // 计算表达式
    double calculate(const std::string& expr) {
        numStack.clear();
        opStack.clear();
        
        for (size_t i = 0; i < expr.length(); i++) {
            char c = expr[i];
            
            if (std::isspace(c)) {
                continue; // 跳过空白字符
            } else if (std::isdigit(c) || c == '.') {
                processNumber(expr, i);
            } else if (std::isalpha(c)) {
                // 尝试处理函数
                if (!processFunction(expr, i)) {
                    // 如果不是函数，可能是常量
                    if (c == 'e' || (c == 'p' && i + 1 < expr.length() && expr[i + 1] == 'i')) {
                        if (c == 'e') {
                            numStack.push(M_E); // 自然对数的底
                        } else {
                            numStack.push(M_PI); // 圆周率
                            i++; // 跳过'i'
                        }
                    } else {
                        throw std::runtime_error("未知的标识符: " + std::string(1, c));
                    }
                }
                i--; // 回退一个位置，因为外层循环会自增
            } else if (c == '(') {
                opStack.push(c);
            } else if (c == ')') {
                processRightParenthesis();
            } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
                processOperator(c);
            } else {
                throw std::runtime_error("表达式包含无效字符");
            }
        }
        
        // 处理剩余的运算符
        while (!opStack.empty()) {
            if (opStack.top() == '(') {
                throw std::runtime_error("括号不匹配");
            }
            calculateTop();
        }
        
        if (numStack.size() != 1) {
            throw std::runtime_error("表达式无效");
        }
        
        return numStack.top();
    }
};

// 主函数
int main() {
    StringCalculator calculator;
    std::string expression;
    
    std::cout << "增强型字符串计算器 (输入'exit'退出)" << std::endl;
    std::cout << "支持的基本运算: +, -, *, /, ^(幂运算), 括号" << std::endl;
    std::cout << "支持的函数: sin, cos, tan, asin, acos, atan, sqrt, log, log10, exp, abs, ceil, floor, round" << std::endl;
    std::cout << "支持的二元函数: max, min, pow" << std::endl;
    std::cout << "支持的常量: e, pi" << std::endl;
    
    // 测试案例
    MySTL::Vector<std::string> testCases = {
        "1 + 2 * 3",
        "(1 + 2) * 3",
        "2 ^ 3 + 4",
        "sin(pi/2)",
        "cos(0)",
        "log(100)",
        "sqrt(16)",
        "max(5, 10)",
        "min(5, 10)",
        "abs(-5)",
        "2 * sin(pi/4) ^ 2"
    };
    
    std::cout << "\n测试案例:" << std::endl;
    for (const auto& test : testCases) {
        try {
            double result = calculator.calculate(test);
            std::cout << test << " = " << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << test << " 错误: " << e.what() << std::endl;
        }
    }
    
    std::cout << "\n开始交互式计算:" << std::endl;
    while (true) {
        std::cout << "\n请输入表达式: ";
        std::getline(std::cin, expression);
        
        if (expression == "exit") {
            break;
        }
        
        try {
            double result = calculator.calculate(expression);
            std::cout << "计算结果: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << "错误: " << e.what() << std::endl;
        }
    }
    
    return 0;
}