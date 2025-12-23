#ifndef GRAPH_IMPLEMENTATION_INCLUDED
#define GRAPH_IMPLEMENTATION_INCLUDED

#include "vector.h"
#include "stack.h"
#include "queue.h"
#include "graph.h"
#include <functional>

namespace MySTL {

template <typename W>
Graph<W>::Graph(int n) : _n(n), _adj(n, n) {}

template <typename W>
int Graph<W>::n() const { return _n; }

template <typename W>
const Vector<Edge<W>>& Graph<W>::neighbors(int u) const { return _adj[u]; }

template <typename W>
void Graph<W>::resize(int n) {
    _n = n;
    _adj = Vector<Vector<Edge<W>>>(n, n, Vector<Edge<W>>());
}

template <typename W>
void Graph<W>::addEdge(int u, int v, W w, bool directed) {
    if (u < 0 || v < 0 || u >= _n || v >= _n) return;
    Edge<W> e1; e1.to = v; e1.w = w;
    _adj[u].insert(_adj[u].size(), e1);
    if (!directed) { Edge<W> e2; e2.to = u; e2.w = w; _adj[v].insert(_adj[v].size(), e2); }
}

template <typename W>
template <typename VST>
void Graph<W>::bfs(int s, VST& visit) const {
    if (s < 0 || s >= _n) return;
    Vector<char> vis(_n, _n, (char)0);
    Queue<int> q; q.push(s); vis[s] = 1;
    while (!q.empty()) {
        int u = q.pop();
        visit(u);
        for (int i = 0; i < _adj[u].size(); ++i) {
            int v = _adj[u][i].to;
            if (!vis[v]) { vis[v] = 1; q.push(v); }
        }
    }
}

template <typename W>
template <typename VST>
void Graph<W>::dfs(int s, VST& visit) const {
    if (s < 0 || s >= _n) return;
    Vector<char> vis(_n, _n, (char)0);
    Stack<int> st; st.push(s);
    while (!st.empty()) {
        int u = st.pop();
        if (vis[u]) continue;
        vis[u] = 1;
        visit(u);
        for (int i = _adj[u].size() - 1; i >= 0; --i) {
            int v = _adj[u][i].to;
            if (!vis[v]) st.push(v);
        }
    }
}

template <typename W>
Vector<W> Graph<W>::dijkstra(int s) const {
    W INF = (W)0x3f3f3f3f;
    Vector<W> dist(_n, _n, INF);
    if (s < 0 || s >= _n) return dist;
    Vector<char> used(_n, _n, (char)0);
    dist[s] = (W)0;
    for (int i = 0; i < _n; i++) {
        int u = -1; W best = INF;
        for (int j = 0; j < _n; j++) {
            if (!used[j] && dist[j] < best) { best = dist[j]; u = j; }
        }
        if (u == -1) break;
        used[u] = 1;
        for (int k = 0; k < _adj[u].size(); ++k) {
            int v = _adj[u][k].to; W w = _adj[u][k].w;
            if (dist[u] + w < dist[v]) dist[v] = dist[u] + w;
        }
    }
    return dist;
}

template <typename W>
Vector<int> Graph<W>::dijkstra_path(int s, int t) const {
    Vector<int> path;
    W INF = (W)0x3f3f3f3f;
    Vector<W> dist(_n, _n, INF);
    if (s < 0 || s >= _n || t < 0 || t >= _n) return path;
    Vector<char> used(_n, _n, (char)0);
    Vector<int> prev(_n, _n, -1);
    dist[s] = (W)0;
    for (int i = 0; i < _n; i++) {
        int u = -1; W best = INF;
        for (int j = 0; j < _n; j++) {
            if (!used[j] && dist[j] < best) { best = dist[j]; u = j; }
        }
        if (u == -1) break;
        used[u] = 1;
        for (int k = 0; k < _adj[u].size(); ++k) {
            int v = _adj[u][k].to; W w = _adj[u][k].w;
            if (dist[u] + w < dist[v]) { dist[v] = dist[u] + w; prev[v] = u; }
        }
    }
    if (s == t) { path.insert(0, s); return path; }
    if (prev[t] == -1) return path;
    Vector<int> rev;
    int v = t;
    while (v != -1) { rev.insert(rev.size(), v); v = prev[v]; }
    for (int i = rev.size() - 1; i >= 0; --i) path.insert(path.size(), rev[i]);
    return path;
}

template <typename W>
template <typename H>
Vector<int> Graph<W>::astar(int s, int t, H h) const {
    Vector<int> path;
    W INF = (W)0x3f3f3f3f;
    if (s < 0 || s >= _n || t < 0 || t >= _n) return path;
    Vector<W> g(_n, _n, INF);
    Vector<W> f(_n, _n, INF);
    Vector<int> prev(_n, _n, -1);
    Vector<char> open(_n, _n, (char)0);
    Vector<char> closed(_n, _n, (char)0);
    g[s] = (W)0;
    f[s] = h(s);
    open[s] = 1;
    while (1) {
        int u = -1; W best = INF;
        for (int i = 0; i < _n; ++i) if (open[i] && f[i] < best) { best = f[i]; u = i; }
        if (u == -1) break;
        if (u == t) break;
        open[u] = 0; closed[u] = 1;
        for (int k = 0; k < _adj[u].size(); ++k) {
            int v = _adj[u][k].to; W w = _adj[u][k].w;
            if (closed[v]) continue;
            W ng = g[u] + w;
            if (!open[v] || ng < g[v]) {
                g[v] = ng;
                f[v] = ng + h(v);
                prev[v] = u;
                open[v] = 1;
            }
        }
    }
    if (s == t) { path.insert(0, s); return path; }
    if (prev[t] == -1) return path;
    Vector<int> rev;
    int v = t;
    while (v != -1) { rev.insert(rev.size(), v); v = prev[v]; }
    for (int i = rev.size() - 1; i >= 0; --i) path.insert(path.size(), rev[i]);
    return path;
}

template <typename W>
Vector<int> Graph<W>::topo_sort() const {
    Vector<int> order;
    Vector<int> indeg(_n, _n, 0);
    for (int u = 0; u < _n; ++u) for (int i = 0; i < _adj[u].size(); ++i) indeg[_adj[u][i].to]++;
    Queue<int> q;
    for (int i = 0; i < _n; ++i) if (indeg[i] == 0) q.push(i);
    while (!q.empty()) {
        int u = q.pop();
        order.insert(order.size(), u);
        for (int i = 0; i < _adj[u].size(); ++i) {
            int v = _adj[u][i].to;
            if (--indeg[v] == 0) q.push(v);
        }
    }
    return order;
}

template <typename W>
Vector<int> Graph<W>::prim_mst(int s) const {
    Vector<int> parent(_n, _n, -1);
    W INF = (W)0x3f3f3f3f;
    Vector<W> key(_n, _n, INF);
    Vector<char> used(_n, _n, (char)0);
    if (s < 0 || s >= _n) return parent;
    key[s] = (W)0;
    for (int i = 0; i < _n; ++i) {
        int u = -1; W best = INF;
        for (int j = 0; j < _n; ++j) if (!used[j] && key[j] < best) { best = key[j]; u = j; }
        if (u == -1) break;
        used[u] = 1;
        for (int k = 0; k < _adj[u].size(); ++k) {
            int v = _adj[u][k].to; W w = _adj[u][k].w;
            if (!used[v] && w < key[v]) { key[v] = w; parent[v] = u; }
        }
    }
    return parent;
}

template <typename W>
Vector<Vector<int>> Graph<W>::biconnected() const {
    Vector<Vector<int>> comps;
    Vector<int> disc(_n, _n, -1), low(_n, _n, -1), parent(_n, _n, -1);
    int tim = 0;
    Vector<int> stU, stV;
    std::function<void(int)> dfs = [&](int u) {
        disc[u] = low[u] = tim++;
        for (int i = 0; i < _adj[u].size(); ++i) {
            int v = _adj[u][i].to;
            if (disc[v] == -1) {
                parent[v] = u;
                stU.insert(stU.size(), u);
                stV.insert(stV.size(), v);
                dfs(v);
                low[u] = low[u] < low[v] ? low[u] : low[v];
                if (low[v] >= disc[u]) {
                    Vector<int> comp;
                    while (!stU.empty()) {
                        int a = stU[stU.size()-1];
                        int b = stV[stV.size()-1];
                        stU.remove(stU.size()-1);
                        stV.remove(stV.size()-1);
                        bool fa = false, fb = false;
                        for (int t = 0; t < comp.size(); ++t) { if (comp[t] == a) fa = true; if (comp[t] == b) fb = true; }
                        if (!fa) comp.insert(comp.size(), a);
                        if (!fb) comp.insert(comp.size(), b);
                        if ((a == u && b == v) || (a == v && b == u)) break;
                    }
                    comps.insert(comps.size(), comp);
                }
            } else if (v != parent[u] && disc[v] < disc[u]) {
                stU.insert(stU.size(), u);
                stV.insert(stV.size(), v);
                low[u] = low[u] < disc[v] ? low[u] : disc[v];
            }
        }
    };
    for (int i = 0; i < _n; ++i) if (disc[i] == -1) dfs(i);
    return comps;
}

template <typename W>
template <typename Score>
Vector<int> Graph<W>::best_first(int s, Score score) const {
    Vector<int> order;
    if (s < 0 || s >= _n) return order;
    Vector<char> open(_n, _n, (char)0), closed(_n, _n, (char)0);
    open[s] = 1;
    while (1) {
        int u = -1; W best = (W)0x3f3f3f3f;
        for (int i = 0; i < _n; ++i) if (open[i]) { W sc = (W)score(i); if (sc < best) { best = sc; u = i; } }
        if (u == -1) break;
        open[u] = 0; closed[u] = 1;
        order.insert(order.size(), u);
        for (int k = 0; k < _adj[u].size(); ++k) {
            int v = _adj[u][k].to;
            if (!closed[v]) open[v] = 1;
        }
    }
    return order;
}

} // namespace MySTL

#endif
