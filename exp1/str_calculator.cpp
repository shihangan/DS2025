#include <iostream>
#include <string>
#include <cctype>
#include <cmath>
#include <map>
#include <functional>
#include "../stack.h"
#include "../vector.h"

using namespace MySTL;

class StringCalculator {
private:
    Stack<double> values;   // 数值栈
    Stack<char> ops;        // 操作符栈

    // 运算符优先级
    int precedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/' || op == '%') return 2;
        if (op == '^') return 3;
        return 0;
    }

    // 计算两个数的运算结果
    double applyOp(double a, double b, char op) {
        switch(op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/':
                if (b == 0) throw std::runtime_error("Division by zero");
                return a / b;
            case '%':
                if (b == 0) throw std::runtime_error("Modulo by zero");
                return std::fmod(a, b);
            case '^': return std::pow(a, b);
        }
        return 0;
    }

    // 解析数字，包括整数和浮点数
    double parseNumber(const std::string& s, size_t& i) {
        size_t start = i;
        bool hasDecimal = false;
        while (i < s.length() && (std::isdigit(s[i]) || s[i] == '.')) {
            if (s[i] == '.') {
                if (hasDecimal) throw std::runtime_error("Invalid number format");
                hasDecimal = true;
            }
            i++;
        }
        double val = std::stod(s.substr(start, i - start));
        return val;
    }

    // 解析函数，如 sin(x), cos(x), tan(x), log(x), sqrt(x)
    double parseFunction(const std::string& s, size_t& i) {
        size_t start = i;
        while (i < s.length() && std::isalpha(s[i])) i++;
        std::string func = s.substr(start, i - start);

        // 支持常量 e 和 pi
        if (func == "e") return std::exp(1.0);
        if (func == "pi") return 3.14159265358979323846;

        // 假设函数格式为 func(expr)
        if (i >= s.length() || s[i] != '(') throw std::runtime_error("Expected '(' after function name");
        i++; // 跳过 '('

        // 解析表达式直到配对的 ')'
        int parenCount = 1;
        std::string expr;
        while (i < s.length() && parenCount > 0) {
            if (s[i] == '(') parenCount++;
            else if (s[i] == ')') parenCount--;
            if (parenCount > 0) expr.push_back(s[i]);
            i++;
        }
        if (parenCount != 0) throw std::runtime_error("Mismatched parentheses in function call");

        double arg = evaluate(expr);

        if (func == "sin") return std::sin(arg);
        if (func == "cos") return std::cos(arg);
        if (func == "tan") return std::tan(arg);
        if (func == "log") {
            if (arg <= 0) throw std::runtime_error("Logarithm of non-positive number");
            return std::log(arg);
        }
        if (func == "sqrt") {
            if (arg < 0) throw std::runtime_error("Square root of negative number");
            return std::sqrt(arg);
        }

        throw std::runtime_error("Unknown function: " + func);
    }

    // 处理一元运算符，如负号
    bool isUnaryMinus(const std::string& s, size_t pos) {
        if (s[pos] != '-') return false;
        if (pos == 0) return true;
        char prev = s[pos - 1];
        return prev == '(' || prev == '+' || prev == '-' || prev == '*' || prev == '/' || prev == '^';
    }

public:
    // 评估表达式
    double evaluate(const std::string& s) {
        size_t i = 0;

        while (i < s.length()) {
            if (std::isspace(s[i])) { i++; continue; }

            if (s[i] == '(') {
                ops.push('(');
                i++;
            } else if (s[i] == ')') {
                while (!ops.empty() && ops.top() != '(') {
                    double b = values.top(); values.pop();
                    double a = values.top(); values.pop();
                    char op = ops.top(); ops.pop();
                    values.push(applyOp(a, b, op));
                }
                if (!ops.empty()) ops.pop();
                i++;
            } else if (std::isdigit(s[i]) || s[i] == '.') {
                values.push(parseNumber(s, i));
            } else if (std::isalpha(s[i])) {
                double val = parseFunction(s, i);
                values.push(val);
            } else if (s[i] == '-') {
                if (isUnaryMinus(s, i)) {
                    i++;
                    double val = parseNumber(s, i);
                    values.push(-val);
                } else {
                    while (!ops.empty() && precedence(ops.top()) >= precedence('-')) {
                        double b = values.top(); values.pop();
                        double a = values.top(); values.pop();
                        char op = ops.top(); ops.pop();
                        values.push(applyOp(a, b, op));
                    }
                    ops.push('-');
                    i++;
                }
            } else if (s[i] == '+' || s[i] == '*' || s[i] == '/' || s[i] == '^' || s[i] == '%') {
                char currentOp = s[i];
                while (!ops.empty() && precedence(ops.top()) >= precedence(currentOp)) {
                    double b = values.top(); values.pop();
                    double a = values.top(); values.pop();
                    char op = ops.top(); ops.pop();
                    values.push(applyOp(a, b, op));
                }
                ops.push(currentOp);
                i++;
            } else {
                throw std::runtime_error(std::string("Unexpected character: ") + s[i]);
            }
        }

        while (!ops.empty()) {
            double b = values.top(); values.pop();
            double a = values.top(); values.pop();
            char op = ops.top(); ops.pop();
            values.push(applyOp(a, b, op));
        }

        if (values.empty()) throw std::runtime_error("Invalid expression");
        return values.top();
    }
};

int main() {
    StringCalculator calc;

    // 测试用例
    MySTL::Vector<std::string> testCases;
    const char* arr[] = {
        "3 + 5",
        "10 + 2 * 6",
        "100 * 2 + 12",
        "100 * ( 2 + 12 )",
        "100 * ( 2 + 12 ) / 14",
        "sin(pi / 2)",
        "cos(0)",
        "tan(pi / 4)",
        "log(e)",
        "sqrt(16)",
        "-5 + 3",
        "-(3 + 2) * 4",
        "3 + 4 * 2 / (1 - 5)^2",
        "10 % 3"
    };
    for (int i=0;i<14;++i) testCases.insert(testCases.size(), std::string(arr[i]));

    for (int i=0;i<testCases.size();++i) {
        const std::string& expr = testCases[i];
        try {
            double result = calc.evaluate(expr);
            std::cout << expr << " = " << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Error evaluating '" << expr << "': " << e.what() << std::endl;
        }
    }

    // 交互模式
    std::cout << "\n请输入表达式（或输入 'exit' 退出）：" << std::endl;
    std::string input;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, input)) break;
        if (input == "exit") break;
        try {
            double result = calc.evaluate(input);
            std::cout << "结果: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << "错误: " << e.what() << std::endl;
        }
    }

    return 0;
}