#include <iostream>
#include <string>
#include "vector.h"
#include "stack.h"
#include "queue.h"
#include "graph.h"

using namespace std;
using namespace MySTL;

static void print_matrix(const Vector<Vector<int>>& M) {
    for (int i = 0; i < M.size(); ++i) {
        for (int j = 0; j < M[i].size(); ++j) {
            cout << M[i][j] << (j + 1 == M[i].size() ? '\n' : ' ');
        }
    }
}

static Graph<int> build_graph1() {
    Graph<int> g(8);
    g.addEdge(0, 1, 4);
    g.addEdge(1, 2, 12);
    g.addEdge(0, 3, 6);
    g.addEdge(3, 6, 2);
    g.addEdge(6, 7, 14);
    g.addEdge(4, 7, 8);
    g.addEdge(5, 7, 3);
    g.addEdge(2, 5, 2);
    g.addEdge(2, 7, 10);
    g.addEdge(4, 5, 5);
    g.addEdge(3, 4, 13);
    g.addEdge(2, 4, 1);
    g.addEdge(4, 6, 11);
    g.addEdge(2, 3, 9);
    g.addEdge(0, 6, 7);
    return g;
}

static Vector<Vector<int>> adj_matrix(const Graph<int>& g, int n, int INF) {
    Vector<Vector<int>> M(n, n, Vector<int>(n, n, INF));
    for (int i = 0; i < n; ++i) M[i][i] = 0;
    for (int u = 0; u < n; ++u) {
        for (int k = 0; k < g.neighbors(u).size(); ++k) {
            int v = g.neighbors(u)[k].to; int w = g.neighbors(u)[k].w;
            if (w < M[u][v]) M[u][v] = w;
            if (w < M[v][u]) M[v][u] = w;
        }
    }
    return M;
}

static void bfs_from_A(const Graph<int>& g) {
    auto visit = [&](int u){ cout << char('A' + u) << (u==7?'\n':' '); };
    Graph<int> gg = g;
    gg.bfs(0, visit);
}

static void dfs_from_A(const Graph<int>& g) {
    auto visit = [&](int u){ cout << char('A' + u) << (u==7?'\n':' '); };
    Graph<int> gg = g;
    gg.dfs(0, visit);
}

static Vector<int> dijkstra_from_A(const Graph<int>& g) {
    Graph<int> gg = g;
    return gg.dijkstra(0);
}

static Vector<int> prim_from_A(const Graph<int>& g) {
    int n = 8; int INF = 0x3f3f3f3f;
    auto M = adj_matrix(g, n, INF);
    Vector<char> inMST(n, n, (char)0);
    Vector<int> key(n, n, INF);
    Vector<int> parent(n, n, -1);
    key[0] = 0; parent[0] = -1;
    for (int cnt = 0; cnt < n; ++cnt) {
        int u = -1; int best = INF;
        for (int i = 0; i < n; ++i) if (!inMST[i] && key[i] < best) { best = key[i]; u = i; }
        if (u == -1) break;
        inMST[u] = 1;
        for (int v = 0; v < n; ++v) {
            int w = M[u][v];
            if (!inMST[v] && w < key[v]) { key[v] = w; parent[v] = u; }
        }
    }
    return parent;
}

struct EdgeU { int u; int v; };

static void tarjan_bcc_art(const Graph<int>& g, int s, Vector<Vector<int>>& bcc, Vector<int>& arts) {
    int n = g.n();
    Vector<int> dfn(n, n, 0);
    Vector<int> low(n, n, 0);
    int tick = 0;
    Stack<EdgeU> st;
    Vector<int> isArt(n, n, 0);
    auto push_edge = [&](int u,int v){ EdgeU e; e.u=u; e.v=v; st.push(e); };
    auto new_bcc = [&](){ bcc.insert(bcc.size(), Vector<int>(0,0,0)); };
    auto add_bcc_vertex = [&](int x){ Vector<int>& comp = bcc[bcc.size()-1]; bool found=false; for(int i=0;i<comp.size();++i) if(comp[i]==x){ found=true; break;} if(!found) comp.insert(comp.size(), x); };
    Vector<int> parent(n, n, -1);
    auto dfs = [&](auto&& self, int u) -> void {
        dfn[u] = low[u] = ++tick;
        int child = 0;
        for (int i = 0; i < g.neighbors(u).size(); ++i) {
            int v = g.neighbors(u)[i].to;
            if (!dfn[v]) {
                parent[v] = u;
                child++;
                push_edge(u, v);
                self(self, v);
                if (low[v] >= dfn[u]) {
                    isArt[u] = 1;
                    new_bcc();
                    while (true) {
                        EdgeU e = st.pop();
                        add_bcc_vertex(e.u);
                        add_bcc_vertex(e.v);
                        if ((e.u == u && e.v == v) || (e.u == v && e.v == u)) break;
                    }
                }
                if (low[v] < low[u]) low[u] = low[v];
            } else if (v != parent[u] && dfn[v] < dfn[u]) {
                if (dfn[v] < low[u]) low[u] = dfn[v];
                push_edge(u, v);
            }
        }
        if (parent[u] == -1 && child < 2) isArt[u] = 0;
    };
    dfs(dfs,s);
    for(int i=0;i<n;++i) if(isArt[i]) arts.insert(arts.size(), i);
}

static Graph<int> build_graph2() {
    Graph<int> g(12);
    auto U = [&](int a,int b){ g.addEdge(a,b,1); };
    U(0,1); U(0,4);
    U(1,2); U(1,5);
    U(2,3); U(2,5);
    U(3,7);
    U(4,5); U(4,8);
    U(5,6); U(5,9); U(5,2); U(5,0); U(5,4); U(5,10); U(5,8);
    U(6,7); U(6,10);
    U(8,9);
    U(9,10);
    U(10,11);
    return g;
}

int main() {
    cout << "Exp3 Graph Algorithms" << endl;
    auto g1 = build_graph1();
    int INF = 0x3f3f3f3f;
    auto M1 = adj_matrix(g1, 8, INF);
    cout << "Adjacency Matrix (Graph1):" << endl;
    print_matrix(M1);
    cout << "BFS from A:" << endl;
    bfs_from_A(g1);
    cout << "DFS from A:" << endl;
    dfs_from_A(g1);
    auto dist = dijkstra_from_A(g1);
    cout << "Shortest distances from A:" << endl;
    for (int i = 0; i < dist.size(); ++i) cout << char('A'+i) << ":" << dist[i] << (i+1==dist.size()?'\n':' ');
    auto parent = prim_from_A(g1);
    cout << "MST edges by Prim from A:" << endl;
    for (int v = 0; v < parent.size(); ++v) if (parent[v] != -1) cout << char('A'+parent[v]) << "-" << char('A'+v) << (v+1==parent.size()?'\n':' ');

    auto g2 = build_graph2();
    Vector<Vector<int>> bccA(0,0,Vector<int>()); Vector<int> artsA(0,0,0); tarjan_bcc_art(g2, 0, bccA, artsA);
    Vector<Vector<int>> bccF(0,0,Vector<int>()); Vector<int> artsF(0,0,0); tarjan_bcc_art(g2, 5, bccF, artsF);
    cout << "Articulation points (start A):" << endl; for(int i=0;i<artsA.size();++i) cout << char('A'+artsA[i]) << (i+1==artsA.size()?'\n':' ');
    cout << "Articulation points (start F):" << endl; for(int i=0;i<artsF.size();++i) cout << char('A'+artsF[i]) << (i+1==artsF.size()?'\n':' ');
    Vector<char> seen(26,26,(char)0);
    bool same=true;
    if(artsA.size()!=artsF.size()) same=false; else { for(int i=0;i<artsA.size();++i){ bool found=false; for(int j=0;j<artsF.size();++j) if(artsA[i]==artsF[j]) {found=true; break;} if(!found){ same=false; break; } } }
    cout << "Articulation sets consistent: " << (same?"Yes":"No") << endl;
    cout << "BCC count (start A): " << bccA.size() << endl;
    cout << "BCC count (start F): " << bccF.size() << endl;
    return 0;
}

