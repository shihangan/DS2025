#include <iostream>
#include <random>
#include <chrono>
#include <cmath>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif
#include "../vector.h"

using namespace std;
using namespace MySTL;

struct Box {
    float x1, y1, x2, y2, score;
    int id;
};

static inline float iou(const Box& a, const Box& b) {
    float ix1 = a.x1 > b.x1 ? a.x1 : b.x1;
    float iy1 = a.y1 > b.y1 ? a.y1 : b.y1;
    float ix2 = a.x2 < b.x2 ? a.x2 : b.x2;
    float iy2 = a.y2 < b.y2 ? a.y2 : b.y2;
    float iw = ix2 - ix1;
    float ih = iy2 - iy1;
    if (iw <= 0 || ih <= 0) return 0.0f;
    float inter = iw * ih;
    float areaA = (a.x2 - a.x1) * (a.y2 - a.y1);
    float areaB = (b.x2 - b.x1) * (b.y2 - b.y1);
    float uni = areaA + areaB - inter;
    if (uni <= 0) return 0.0f;
    return inter / uni;
}

enum SortAlgo { QUICK, MERGE, HEAP, INSERTION, BUBBLE };

template<typename T>
static inline void vswap(T& a, T& b) { T t = a; a = b; b = t; }

struct CmpScoreLess {
    bool operator()(const Box& a, const Box& b) const { return a.score < b.score; }
};

template<typename T, typename Compare>
static int part(Vector<T>& a, int lo, int hi, Compare comp) {
    T pivot = a[hi - 1];
    int i = lo;
    for (int j = lo; j < hi - 1; ++j) {
        if (comp(pivot, a[j])) { vswap(a[i], a[j]); ++i; }
    }
    vswap(a[i], a[hi - 1]);
    return i;
}

template<typename T, typename Compare>
static void qrec(Vector<T>& a, int lo, int hi, Compare comp) {
    if (hi - lo <= 1) return;
    int p = part(a, lo, hi, comp);
    qrec(a, lo, p, comp);
    qrec(a, p + 1, hi, comp);
}

template<typename T, typename Compare>
static void sort_quick(Vector<T>& a, Compare comp) { qrec(a, 0, a.size(), comp); }

template<typename T, typename Compare>
static void merge(Vector<T>& a, int lo, int mid, int hi, Compare comp) {
    int n = hi - lo;
    T* tmp = new T[n];
    int i = lo, j = mid, k = 0;
    while (i < mid && j < hi) {
        if (comp(a[i], a[j])) { tmp[k++] = a[j++]; }
        else { tmp[k++] = a[i++]; }
    }
    while (i < mid) tmp[k++] = a[i++];
    while (j < hi) tmp[k++] = a[j++];
    for (int t = 0; t < n; ++t) a[lo + t] = tmp[t];
    delete[] tmp;
}

template<typename T, typename Compare>
static void mrec(Vector<T>& a, int lo, int hi, Compare comp) {
    if (hi - lo <= 1) return;
    int mid = (lo + hi) >> 1;
    mrec(a, lo, mid, comp);
    mrec(a, mid, hi, comp);
    merge(a, lo, mid, hi, comp);
}

template<typename T, typename Compare>
static void sort_merge(Vector<T>& a, Compare comp) { mrec(a, 0, a.size(), comp); }

template<typename T, typename Compare>
static void heapify(Vector<T>& a, int n, int i, Compare comp) {
    int largest = i;
    int l = (i << 1) + 1;
    int r = l + 1;
    if (l < n && comp(a[largest], a[l])) largest = l;
    if (r < n && comp(a[largest], a[r])) largest = r;
    if (largest != i) { vswap(a[i], a[largest]); heapify(a, n, largest, comp); }
}

template<typename T, typename Compare>
static void sort_heap(Vector<T>& a, Compare comp) {
    int n = a.size();
    for (int i = (n >> 1) - 1; i >= 0; --i) heapify(a, n, i, comp);
    for (int i = n - 1; i > 0; --i) { vswap(a[0], a[i]); heapify(a, i, 0, comp); }
    for (int i = 0, j = n - 1; i < j; ++i, --j) vswap(a[i], a[j]);
}

template<typename T, typename Compare>
static void sort_insertion(Vector<T>& a, Compare comp) {
    int n = a.size();
    for (int i = 1; i < n; ++i) {
        T key = a[i];
        int j = i - 1;
        while (j >= 0 && comp(key, a[j])) { a[j + 1] = a[j]; --j; }
        a[j + 1] = key;
    }
}

template<typename T, typename Compare>
static void sort_bubble(Vector<T>& a, Compare comp) {
    int n = a.size();
    bool swapped = true;
    while (swapped) {
        swapped = false;
        for (int i = 1; i < n; ++i) {
            if (comp(a[i - 1], a[i])) { vswap(a[i - 1], a[i]); swapped = true; }
        }
        --n; if (n <= 1) break;
    }
}

static void apply_sort(Vector<Box>& boxes, SortAlgo algo) {
    CmpScoreLess cmp;
    if (algo == QUICK) sort_quick(boxes, cmp);
    else if (algo == MERGE) sort_merge(boxes, cmp);
    else if (algo == HEAP) sort_heap(boxes, cmp);
    else if (algo == INSERTION) sort_insertion(boxes, cmp);
    else sort_bubble(boxes, cmp);
}

static Vector<Box> nms(Vector<Box> boxes, float thr, SortAlgo algo) {
    apply_sort(boxes, algo);
    Vector<Box> keep;
    Vector<char> sup;
    for (int i = 0; i < boxes.size(); ++i) sup.insert(sup.size(), 0);
    for (int i = 0; i < boxes.size(); ++i) {
        if (sup[i]) continue;
        keep.insert(keep.size(), boxes[i]);
        for (int j = i + 1; j < boxes.size(); ++j) {
            if (sup[j]) continue;
            if (iou(boxes[i], boxes[j]) > thr) sup[j] = 1;
        }
    }
    return keep;
}

static Vector<Box> nms_grid(Vector<Box> boxes, float thr, SortAlgo algo, float cell) {
    apply_sort(boxes, algo);
    float maxX = 0.0f, maxY = 0.0f;
    for (int i = 0; i < boxes.size(); ++i) {
        if (boxes[i].x2 > maxX) maxX = boxes[i].x2;
        if (boxes[i].y2 > maxY) maxY = boxes[i].y2;
    }
    int gw = (int)std::ceil(maxX / cell); if (gw < 1) gw = 1;
    int gh = (int)std::ceil(maxY / cell); if (gh < 1) gh = 1;
    int gsz = gw * gh;
    Vector<Vector<int>> grid;
    for (int k = 0; k < gsz; ++k) { Vector<int> lst; grid.insert(grid.size(), lst); }
    for (int i = 0; i < boxes.size(); ++i) {
        float cx = (boxes[i].x1 + boxes[i].x2) * 0.5f;
        float cy = (boxes[i].y1 + boxes[i].y2) * 0.5f;
        int gx = (int)(cx / cell); if (gx >= gw) gx = gw - 1; if (gx < 0) gx = 0;
        int gy = (int)(cy / cell); if (gy >= gh) gy = gh - 1; if (gy < 0) gy = 0;
        int idx = gy * gw + gx;
        grid[idx].insert(grid[idx].size(), i);
    }
    Vector<Box> keep;
    Vector<char> sup; for (int i = 0; i < boxes.size(); ++i) sup.insert(sup.size(), 0);
    for (int i = 0; i < boxes.size(); ++i) {
        if (sup[i]) continue;
        keep.insert(keep.size(), boxes[i]);
        float cx = (boxes[i].x1 + boxes[i].x2) * 0.5f;
        float cy = (boxes[i].y1 + boxes[i].y2) * 0.5f;
        int gx = (int)(cx / cell); if (gx >= gw) gx = gw - 1; if (gx < 0) gx = 0;
        int gy = (int)(cy / cell); if (gy >= gh) gy = gh - 1; if (gy < 0) gy = 0;
        int gx0 = gx - 1; if (gx0 < 0) gx0 = 0;
        int gy0 = gy - 1; if (gy0 < 0) gy0 = 0;
        int gx1 = gx + 1; if (gx1 >= gw) gx1 = gw - 1;
        int gy1 = gy + 1; if (gy1 >= gh) gy1 = gh - 1;
        for (int yy = gy0; yy <= gy1; ++yy) {
            for (int xx = gx0; xx <= gx1; ++xx) {
                int idx = yy * gw + xx;
                for (int t = 0; t < grid[idx].size(); ++t) {
                    int j = grid[idx][t];
                    if (j <= i) continue;
                    if (sup[j]) continue;
                    if (iou(boxes[i], boxes[j]) > thr) sup[j] = 1;
                }
            }
        }
    }
    return keep;
}

static Vector<Box> gen_random(int n, int W, int H, unsigned seed) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> ux(0.0f, (float)W);
    std::uniform_real_distribution<float> uy(0.0f, (float)H);
    std::uniform_real_distribution<float> us(5.0f, (float)(W > H ? H : W) / 5.0f);
    std::uniform_real_distribution<float> sc(0.0f, 1.0f);
    Vector<Box> boxes;
    for (int i = 0; i < n; ++i) {
        float cx = ux(rng);
        float cy = uy(rng);
        float w = us(rng);
        float h = us(rng);
        float x1 = cx - w * 0.5f; if (x1 < 0) x1 = 0;
        float y1 = cy - h * 0.5f; if (y1 < 0) y1 = 0;
        float x2 = cx + w * 0.5f; if (x2 > W) x2 = (float)W;
        float y2 = cy + h * 0.5f; if (y2 > H) y2 = (float)H;
        Box b; b.x1 = x1; b.y1 = y1; b.x2 = x2; b.y2 = y2; b.score = sc(rng); b.id = i;
        boxes.insert(boxes.size(), b);
    }
    return boxes;
}

static Vector<Box> gen_cluster(int n, int W, int H, int K, unsigned seed) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> ucx(0.1f * W, 0.9f * W);
    std::uniform_real_distribution<float> ucy(0.1f * H, 0.9f * H);
    std::uniform_real_distribution<float> us(5.0f, (float)(W > H ? H : W) / 8.0f);
    std::uniform_real_distribution<float> sc(0.0f, 1.0f);
    Vector<float> centers;
    for (int k = 0; k < K; ++k) {
        centers.insert(centers.size(), ucx(rng));
        centers.insert(centers.size(), ucy(rng));
    }
    std::normal_distribution<float> nx(0.0f, W * 0.05f);
    std::normal_distribution<float> ny(0.0f, H * 0.05f);
    Vector<Box> boxes;
    for (int i = 0; i < n; ++i) {
        int c = i % K;
        float cx = centers[c * 2] + nx(rng);
        float cy = centers[c * 2 + 1] + ny(rng);
        float w = us(rng);
        float h = us(rng);
        float x1 = cx - w * 0.5f; if (x1 < 0) x1 = 0;
        float y1 = cy - h * 0.5f; if (y1 < 0) y1 = 0;
        float x2 = cx + w * 0.5f; if (x2 > W) x2 = (float)W;
        float y2 = cy + h * 0.5f; if (y2 > H) y2 = (float)H;
        Box b; b.x1 = x1; b.y1 = y1; b.x2 = x2; b.y2 = y2; b.score = sc(rng); b.id = i;
        boxes.insert(boxes.size(), b);
    }
    return boxes;
}

static long long run_nms(Vector<Box>& boxes, SortAlgo algo, float thr) {
    auto t0 = std::chrono::high_resolution_clock::now();
    Vector<Box> keep = nms(boxes, thr, algo);
    auto t1 = std::chrono::high_resolution_clock::now();
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    return dt;
}

static long long run_nms_grid(Vector<Box>& boxes, SortAlgo algo, float thr, float cell) {
    auto t0 = std::chrono::high_resolution_clock::now();
    Vector<Box> keep = nms_grid(boxes, thr, algo, cell);
    auto t1 = std::chrono::high_resolution_clock::now();
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    return dt;
}

static void benchmark() {
    int W = 1000, H = 1000;
    Vector<int> sizes;
    sizes.insert(sizes.size(), 100);
    sizes.insert(sizes.size(), 1000);
    sizes.insert(sizes.size(), 3000);
    float thr = 0.5f;
    SortAlgo algos[5] = { QUICK, MERGE, HEAP, INSERTION, BUBBLE };
    const char* an[5] = { "快速排序", "归并排序", "堆排序", "插入排序", "冒泡排序" };
    float cell = 50.0f;
    cout << "分布: 随机" << endl;
    for (int si = 0; si < sizes.size(); ++si) {
        int n = sizes[si];
        cout << "数量: " << n << endl;
        Vector<Box> boxes = gen_random(n, W, H, 12345u + n);
        for (int ai = 0; ai < 5; ++ai) {
            Vector<Box> cpy(boxes);
            long long ms_base = run_nms(cpy, algos[ai], thr);
            Vector<Box> cpy2(boxes);
            long long ms_grid = run_nms_grid(cpy2, algos[ai], thr, cell);
            cout << an[ai] << " - 基础NMS: " << ms_base << " ms" << endl;
            cout << an[ai] << " - 栅格NMS: " << ms_grid << " ms" << endl;
        }
    }
    cout << "分布: 聚集" << endl;
    for (int si = 0; si < sizes.size(); ++si) {
        int n = sizes[si];
        cout << "数量: " << n << endl;
        Vector<Box> boxes = gen_cluster(n, W, H, 5, 54321u + n);
        for (int ai = 0; ai < 5; ++ai) {
            Vector<Box> cpy(boxes);
            long long ms_base = run_nms(cpy, algos[ai], thr);
            Vector<Box> cpy2(boxes);
            long long ms_grid = run_nms_grid(cpy2, algos[ai], thr, cell);
            cout << an[ai] << " - 基础NMS: " << ms_base << " ms" << endl;
            cout << an[ai] << " - 栅格NMS: " << ms_grid << " ms" << endl;
        }
    }
}

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif
    benchmark();
    return 0;
}
