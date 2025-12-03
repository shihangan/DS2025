#include <iostream>
#include <fstream>
#include <string>
#include "vector.h"
#include <cctype>
#include <climits>
#include <cstring>
#include "tree.h"

using namespace std;
using namespace MySTL;

typedef int Rank;

class Bitmap {
private:
    unsigned char* M;
    Rank N, _sz;
    void init(Rank n) { M = new unsigned char[N = (n + 7) / 8]; memset(M, 0, N); _sz = 0; }
    void expand(Rank k) {
        if (k < 8 * N) return;
        Rank oldN = N; unsigned char* oldM = M;
        init(2 * k);
        memcpy(M, oldM, oldN);
        delete[] oldM;
    }
public:
    Bitmap(Rank n = 8) { init(n); }
    Bitmap(const Bitmap& other) { 
        M = new unsigned char[N = other.N]; 
        memcpy(M, other.M, other.N); 
        _sz = other._sz; }
    Bitmap& operator=(const Bitmap& other) { 
        if (this == &other) return *this; 
        unsigned char* newM = new unsigned char[other.N]; 
        memcpy(newM, other.M, other.N); 
        delete[] M; 
        M = newM; 
        N = other.N; 
        _sz = other._sz; 
        return *this; }
    ~Bitmap() { delete[] M; M = nullptr; _sz = 0; }
    Rank size() const { return _sz; }
    void set(Rank k) { expand(k); _sz++; M[k >> 3] |= (0x80 >> (k & 0x07)); }
    void clear(Rank k) { expand(k); if (_sz > 0) _sz--; M[k >> 3] &= ~(0x80 >> (k & 0x07)); }
    bool test(Rank k) const { return M[k >> 3] & (0x80 >> (k & 0x07)); }
    char* bits2string(Rank n) const { 
        char* s = new char[n + 1]; 
        s[n] = '\0'; 
        for (Rank i = 0; i < n; i++) s[i] = test(i) ? '1' : '0'; 
        return s; 
    }
};

struct HuffCode { 
    Bitmap bits;
    Rank len;
    HuffCode(): bits(8), len(0) {} void push(bool b) { 
        if (b) bits.set(len); 
        else bits.clear(len); len++; 
    } 
    string str() const { 
        char* s = bits.bits2string(len); 
        string r(s); 
        delete[] s; 
        return r; 
    } 
};

struct HuffItem { char ch; int freq; };

static string read_text_or_sample(const string& path) { 
    ifstream fin(path, ios::in | ios::binary); 
    if (fin) { 
        string s; fin.seekg(0, ios::end); 
        size_t n = fin.tellg(); 
        fin.seekg(0); 
        s.resize(n); 
        fin.read(&s[0], n); 
        return s; 
    } 
    string s = "I have a dream that one day this nation will rise up and live out the true meaning of its creed: we hold these truths to be self-evident, that all men are created equal. I have a dream."; 
    return s; 
}

static Vector<int> letter_freq(const string& s) { 
    Vector<int> f(26, 26, 0); 
    for (unsigned char c : s) {
        char x = tolower(c); 
        if (x >= 'a' && x <= 'z') f[x - 'a'] = f[x - 'a'] + 1; 
    } 
    return f; 
}

static int find_min_index(const Vector<BinaryTree<HuffItem>*>& nodes) {
    int idx = -1; int best = INT_MAX;
    for (int i = 0; i < nodes.size(); ++i) {
        BinaryTree<HuffItem>* t = nodes[i];
        if (!t || !t->root()) continue;
        int f = t->root()->data.freq;
        if (f < best) { best = f; idx = i; }
    }
    return idx;
}

static BinaryTree<HuffItem>* build_huff(const Vector<int>& freq) {
    Vector<BinaryTree<HuffItem>*> nodes(26, 0, (BinaryTree<HuffItem>*)nullptr);
    for (int i = 0; i < 26; ++i) {
        if (freq[i] > 0) {
            auto* t = new BinaryTree<HuffItem>();
            t->insertAsRoot(HuffItem{ char('a' + i), freq[i] });
            nodes.insert(nodes.size(), t);
        }
    }
    if (nodes.size() == 0) return new BinaryTree<HuffItem>();
    while (nodes.size() > 1) {
        int i1 = find_min_index(nodes);
        auto* t1 = nodes.remove(i1);
        int i2 = find_min_index(nodes);
        auto* t2 = nodes.remove(i2);
        auto* t = new BinaryTree<HuffItem>();
        auto* r = t->insertAsRoot(HuffItem{ '\0', t1->root()->data.freq + t2->root()->data.freq });
        t->attachLeft(r, *t1);
        t->attachRight(r, *t2);
        delete t1; delete t2;
        nodes.insert(nodes.size(), t);
    }
    return nodes[0];
}

static void gen_codes(TreeNode<HuffItem>* x, HuffCode& cur, Vector<HuffCode>& codes) { 
    if (!x) return; 
    if (!x->left && !x->right) { 
        if (x->data.ch >= 'a' && x->data.ch <= 'z') codes[x->data.ch - 'a'] = cur; return; 
    } 
    cur.push(false); 
    gen_codes(x->left, cur, codes); 
    cur.len--; 
    cur.push(true); 
    gen_codes(x->right, cur, codes); 
    cur.len--; 
}

static string encode_word(const string& w, const Vector<HuffCode>& codes) { 
    string r; 
    for (size_t k = 0; k < w.size(); ++k) { 
        char x = tolower(static_cast<unsigned char>(w[k])); 
        if (x >= 'a' && x <= 'z') { 
            const HuffCode& hc = codes[x - 'a']; 
            if (hc.len > 0) r += hc.str(); 
        } 
    } 
    return r; 
}

int main() {
    cout << "Huffman Coding (exp2)" << endl;
    string text = read_text_or_sample("exp2/I_have_a_dream.txt");
    auto freq = letter_freq(text);
    auto* ht = build_huff(freq);
    Vector<HuffCode> codes(26, 26, HuffCode()); HuffCode cur; gen_codes(ht->root(), cur, codes);
    cout << "Codes:" << endl; for (int i = 0; i < 26; ++i) { if (codes[i].len > 0) cout << char('a' + i) << ": " << codes[i].str() << endl; }
    Vector<string> words(4, 4, string()); words[0] = "dream"; words[1] = "have"; words[2] = "martin"; words[3] = "king";
    for (int i = 0; i < words.size(); ++i) { const string& w = words[i]; cout << w << " => " << encode_word(w, codes) << endl; }
    delete ht;
    return 0;
}