#ifndef MYSTL_GRAPH_H
#define MYSTL_GRAPH_H

#include "vector.h"

namespace MySTL {

template <typename W>
struct Edge { int to; W w; };

template <typename W = int>
class Graph {
private:
    int _n;
    Vector<Vector<Edge<W>>> _adj;

public:
    Graph(int n = 0);
    int n() const;
    const Vector<Edge<W>>& neighbors(int u) const;
    void resize(int n);
    void addEdge(int u, int v, W w = W(1), bool directed = false);
    template <typename VST> void bfs(int s, VST& visit) const;
    template <typename VST> void dfs(int s, VST& visit) const;
    Vector<W> dijkstra(int s) const;
    Vector<int> dijkstra_path(int s, int t) const;
    template <typename H>
    Vector<int> astar(int s, int t, H h) const;
    Vector<int> topo_sort() const;
    Vector<int> prim_mst(int s = 0) const;
    Vector<Vector<int>> biconnected() const;
    template <typename Score>
    Vector<int> best_first(int s, Score score) const;
};

} // namespace MySTL

#include "graph.cpp"

#endif
