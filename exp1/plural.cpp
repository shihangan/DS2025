#include <iostream>
#include <cmath>
#include <ctime>
#include <random>
#include <algorithm>
#include <functional>
#include <string>
#include "../vector.h"

using namespace MySTL;

// 复数类定义
class plural {
private:
    double real;    // 实部
    double imag;    // 虚部
    double mod;     // 模

    // 计算模的辅助函数
    void calculateModulus() {
        mod = std::sqrt(real * real + imag * imag);
    }

public:
    // 构造函数
    plural() : real(0.0), imag(0.0), mod(0.0) {}
    plural(double r, double i) : real(r), imag(i) {
        calculateModulus();
    }
    
    // 获取实部和虚部
    double getReal() const { return real; }
    double getImag() const { return imag; }
    
    // 设置实部和虚部
    void setReal(double r) { 
        real = r; 
        calculateModulus();
    }
    void setImag(double i) { 
        imag = i; 
        calculateModulus();
    }
    
    // 获取模
    double modulus() const {
        return mod;
    }
    
    // 重载相等运算符，用于查找
    bool operator==(const plural& other) const {
        return real == other.real && imag == other.imag;
    }
    
    // 重载不等运算符
    bool operator!=(const plural& other) const {
        return !(*this == other);
    }
    
    // 输出运算符重载
    friend std::ostream& operator<<(std::ostream& os, const plural& p) {
        os << p.real;
        if (p.imag >= 0)
            os << " + " << p.imag << "i";
        else
            os << " - " << -p.imag << "i";
        return os;
    }
};

// 复数比较器：先比较模，模相同时比较实部
struct PluralComparator {
    bool operator()(const plural& a, const plural& b) const {
        double mod_a = a.modulus();
        double mod_b = b.modulus();
        return mod_a < mod_b;
    }
};

// 生成随机复数向量
MySTL::Vector<plural> generateRandomPluralVector(int size, double min_val = -10.0, double max_val = 10.0) {
    MySTL::Vector<plural> vec;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(min_val, max_val);
    
    for (int i = 0; i < size; ++i) {
        double real = dist(gen);
        double imag = dist(gen);
        vec.insert(plural(real, imag)); // 使用MySTL::Vector的insert方法
    }
    
    return vec;
}

// 打印复数向量
void printPluralVector(const MySTL::Vector<plural>& vec, const std::string& label = "") {
    if (!label.empty()) {
        std::cout << label << ": ";
    }
    std::cout << "[ ";
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << " ]" << std::endl;
}

// 置乱复数向量
void shufflePluralVector(MySTL::Vector<plural>& vec) {
    vec.unsort(); // 使用MySTL::Vector的unsort方法
}

// 查找复数（实部和虚部均相同）
bool findPlural(const MySTL::Vector<plural>& vec, const plural& p) {
    return vec.find(p) != -1; // 使用MySTL::Vector的find方法
}

// 唯一化复数向量（去除重复项）
void uniquifyPluralVector(MySTL::Vector<plural>& vec) {
    vec.deduplicate(); // 使用MySTL::Vector的deduplicate方法
}

// 起泡排序
template<typename T>
void bubbleSort(MySTL::Vector<T>& vec, const std::function<bool(const T&, const T&)>& comp) {
    clock_t start = clock();
    
    int n = vec.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (comp(vec[j+1], vec[j])) {
                std::swap(vec[j], vec[j+1]);
            }
        }
    }
    
    clock_t end = clock();
    std::cout << "起泡排序耗时: " << (double)(end - start) / CLOCKS_PER_SEC << " 秒" << std::endl;
}

// 归并排序辅助函数
template<typename T>
void merge(MySTL::Vector<T>& vec, int left, int mid, int right, 
           const std::function<bool(const T&, const T&)>& comp) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // 创建临时数组
    MySTL::Vector<T> L, R;
    
    // 复制数据到临时数组
    for (int i = 0; i < n1; ++i)
        L.insert(vec[left + i]);
    for (int j = 0; j < n2; ++j)
        R.insert(vec[mid + 1 + j]);
    
    // 合并临时数组
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (!comp(R[j], L[i])) {
            vec[k] = L[i];
            i++;
        } else {
            vec[k] = R[j];
            j++;
        }
        k++;
    }
    
    // 复制剩余元素
    while (i < n1) {
        vec[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        vec[k] = R[j];
        j++;
        k++;
    }
}

// 归并排序实现
template<typename T>
void mergeSortImpl(MySTL::Vector<T>& vec, int left, int right, 
                  const std::function<bool(const T&, const T&)>& comp) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortImpl(vec, left, mid, comp);
        mergeSortImpl(vec, mid + 1, right, comp);
        merge(vec, left, mid, right, comp);
    }
}

// 归并排序
template<typename T>
void mergeSort(MySTL::Vector<T>& vec, const std::function<bool(const T&, const T&)>& comp) {
    clock_t start = clock();
    
    if (vec.size() > 1) {
        mergeSortImpl(vec, 0, vec.size() - 1, comp);
    }
    
    clock_t end = clock();
    std::cout << "归并排序耗时: " << (double)(end - start) / CLOCKS_PER_SEC << " 秒" << std::endl;
}

// 区间查找算法，查找模在[m1,m2)范围内的元素
MySTL::Vector<plural> findInModulusRange(const MySTL::Vector<plural>& vec, double m1, double m2) {
    MySTL::Vector<plural> result;
    
    for (int i = 0; i < vec.size(); ++i) {
        double mod = vec[i].modulus();
        if (mod >= m1 && mod < m2) {
            result.insert(vec[i]);
        }
    }
    
    return result;
}

// 主函数，测试复数向量的各种操作
int main() {
    std::cout << "复数向量操作测试：" << std::endl;
    std::cout << "==============================" << std::endl;
    
    // 生成随机复数向量
    MySTL::Vector<plural> vec = generateRandomPluralVector(10);
    printPluralVector(vec, "原始向量");
    
    // 测试查找功能
    plural target(3.0, 4.0);
    std::cout << "\n查找复数 " << target << ": ";
    if (findPlural(vec, target)) {
        std::cout << "找到" << std::endl;
    } else {
        std::cout << "未找到" << std::endl;
    }
    
    // 测试唯一化
    vec.insert(vec[0]); // 插入重复元素
    printPluralVector(vec, "插入重复元素后");
    uniquifyPluralVector(vec);
    printPluralVector(vec, "唯一化后");
    
    // 测试排序
    PluralComparator comp;
    std::function<bool(const plural&, const plural&)> compFunc = comp;
    
    shufflePluralVector(vec);
    printPluralVector(vec, "打乱后");
    
    bubbleSort(vec, compFunc);
    printPluralVector(vec, "冒泡排序后");
    
    shufflePluralVector(vec);
    mergeSort(vec, compFunc);
    printPluralVector(vec, "归并排序后");
    
    // 测试区间查找
    MySTL::Vector<plural> rangeResult = findInModulusRange(vec, 2.0, 8.0);
    printPluralVector(rangeResult, "模在[2.0, 8.0)范围内的复数");
    
    return 0;
}