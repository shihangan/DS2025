#include <iostream>
#include <cmath>
#include <ctime>
#include <random>
#include <algorithm>
#include <functional>
#include <string>
#include "../vector.h"

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
        
        if (std::abs(mod_a - mod_b) < 1e-10) {
            return a.getReal() < b.getReal();
        }
        return mod_a < mod_b;
    }
};

// 生成随机复数向量
MySTL::vector<plural> generateRandomPluralVector(int size, double min_val = -10.0, double max_val = 10.0) {
    MySTL::vector<plural> vec;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(min_val, max_val);
    
    for (int i = 0; i < size; ++i) {
        double real = dist(gen);
        double imag = dist(gen);
        vec.push_back(plural(real, imag));
    }
    
    return vec;
}

// 打印复数向量
void printPluralVector(const MySTL::vector<plural>& vec, const std::string& label = "") {
    if (!label.empty()) {
        std::cout << label << ": ";
    }
    std::cout << "[ ";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << " ]" << std::endl;
}

// 置乱复数向量
void shufflePluralVector(MySTL::vector<plural>& vec) {
    std::random_device rd;
    std::mt19937 g(rd());
    
    for (size_t i = vec.size() - 1; i > 0; --i) {
        std::uniform_int_distribution<size_t> dist(0, i);
        size_t j = dist(g);
        std::swap(vec[i], vec[j]);
    }
}

// 查找复数（实部和虚部均相同）
bool findPlural(const MySTL::vector<plural>& vec, const plural& p) {
    return MySTL::linear_search(vec, p) != vec.end();
}

// 唯一化复数向量（去除重复项）
void uniquifyPluralVector(MySTL::vector<plural>& vec) {
    MySTL::vector<plural> result;
    
    for (size_t i = 0; i < vec.size(); ++i) {
        bool isDuplicate = false;
        for (size_t j = 0; j < result.size(); ++j) {
            if (vec[i] == result[j]) {
                isDuplicate = true;
                break;
            }
        }
        if (!isDuplicate) {
            result.push_back(vec[i]);
        }
    }
    
    vec = std::move(result);
}

// 起泡排序
template<typename T>
void bubbleSort(MySTL::vector<T>& vec, const std::function<bool(const T&, const T&)>& comp) {
    clock_t start = clock();
    
    size_t n = vec.size();
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
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
void merge(MySTL::vector<T>& vec, size_t left, size_t mid, size_t right, 
           const std::function<bool(const T&, const T&)>& comp) {
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;
    
    // 创建临时数组
    MySTL::vector<T> L, R;
    
    // 复制数据到临时数组
    for (size_t i = 0; i < n1; ++i)
        L.push_back(vec[left + i]);
    for (size_t j = 0; j < n2; ++j)
        R.push_back(vec[mid + 1 + j]);
    
    // 合并临时数组
    size_t i = 0, j = 0, k = left;
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
void mergeSortImpl(MySTL::vector<T>& vec, size_t left, size_t right, 
                  const std::function<bool(const T&, const T&)>& comp) {
    if (left < right) {
        size_t mid = left + (right - left) / 2;
        mergeSortImpl(vec, left, mid, comp);
        mergeSortImpl(vec, mid + 1, right, comp);
        merge(vec, left, mid, right, comp);
    }
}

// 归并排序
template<typename T>
void mergeSort(MySTL::vector<T>& vec, const std::function<bool(const T&, const T&)>& comp) {
    clock_t start = clock();
    
    if (vec.size() > 1) {
        mergeSortImpl(vec, 0, vec.size() - 1, comp);
    }
    
    clock_t end = clock();
    std::cout << "归并排序耗时: " << (double)(end - start) / CLOCKS_PER_SEC << " 秒" << std::endl;
}

// 区间查找算法，查找模在[m1,m2)范围内的元素
MySTL::vector<plural> findInModulusRange(const MySTL::vector<plural>& vec, double m1, double m2) {
    MySTL::vector<plural> result;
    
    for (size_t i = 0; i < vec.size(); ++i) {
        double mod = vec[i].modulus();
        if (mod >= m1 && mod < m2) {
            result.push_back(vec[i]);
        }
    }
    
    return result;
}