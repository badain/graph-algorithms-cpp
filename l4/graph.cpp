#define BOOST_ALLOW_DEPRECATED_HEADERS
#include "graph.h"
using namespace boost;

Graph read_graph(istream& in){
    
    // stores initial input
    typename graph_traits<Graph>::vertices_size_type num_vertices; in >> num_vertices;
    size_t num_edges; in >> num_edges;

    // creates graph with num_vertices vtxs
    Graph graph(num_vertices);
    // Graph population
    while(num_edges--) {
        int u, v; in >> u >> v;
        add_edge(--u, --v, graph);
    }

    return graph;
    
}