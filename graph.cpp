#ifndef GRAPH_IMPLEMENTATION_INCLUDED
#define GRAPH_IMPLEMENTATION_INCLUDED

#include "vector.h"
#include "stack.h"
#include "queue.h"
#include "graph.h"

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

} // namespace MySTL

#endif
