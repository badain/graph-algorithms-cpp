// dependencies
#include <iostream>
#include <vector>

#include <boost/range/iterator_range.hpp>
#include "graph.h"

// namespaces
using namespace std;
using namespace boost;
using boost::make_iterator_range;

typedef std::vector<Vertex> Path; // path: sequence of vtx

void extend_path_from(const Vertex& start, const Graph& graph, Path& path, vector<bool>& visited) {
    
    path.push_back(start); // adds vtx to path
    visited[start] = true; // marks as visited

    for(const auto& edge : make_iterator_range(out_edges(start, graph))) { // checks every neighbour of start for a unvisited vtx
        auto vtx = target(edge, graph); // gets neighbour of vtx
        if(!visited[vtx]) { // if its not visited, extends the path
            extend_path_from(vtx, graph, path, visited);
            return;
        }
    }
}

Path maximal_path_from(const Vertex& start, const Graph& graph) {
    // visited vtx by the algorithm
    vector<bool> visited(num_vertices(graph), false);

    // path extension
    Path path;
    extend_path_from(start, graph, path, visited);

    return path;
}

int main(int argc, char** argv) {
    // input reading
    Graph graph = std::move(read_graph(cin)); // move() avoids copies
    Vertex start; cin >> start;

    // maximal path algorithm
    auto path(maximal_path_from(--start, graph));

    // output
    cout << "Maximal path start at " << start+1 << "found:";
    // prints vertices in a path
    for(const auto& vertex : path) {
        cout << ' ' << vertex+1;
    }
    
    return EXIT_SUCCESS;
}