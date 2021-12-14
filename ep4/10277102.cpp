/* DEPENDENCIES */
#include <cstdlib> // size_t EXIT_SUCCESS EXIT_FAILURE

#include <iostream> // cin cout
#include <tuple>    // tie ignore
#include <queue>    // queue

#define BOOST_ALLOW_DEPRECATED_HEADERS // silence warnings
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

using namespace std;
using namespace boost;

/* DIGRAPH */
struct BundledVertex
{
  double d;
  bool color;
  BundledVertex() : d(0.0), color(false) {}
};
struct BundledArc
{
  double capacity;
  BundledArc() : capacity(0.0) {}
};

typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::directedS,
                              BundledVertex,
                              BundledArc> Digraph;
typedef boost::graph_traits<Digraph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Digraph>::edge_descriptor   Arc;


/* DIGRAPH ITERATORS */
typedef boost::graph_traits<Digraph>::vertex_iterator    vtx_iterator_type; // vtx
typedef boost::graph_traits<Digraph>::edge_iterator      arc_iterator_type; // arc
typedef boost::graph_traits<Digraph>::adjacency_iterator adj_iterator_type; // adj
Vertex null_vtx = boost::graph_traits<Digraph>::null_vertex();

/* INPUT */
Digraph read_network(istream& is) {

    typename graph_traits<Digraph>::vertices_size_type n; is >> n; // number of vertices

    Digraph network(n);

    size_t m; is >> m; // number of arcs

    Vertex source; is >> source; // source vtx
    Vertex target; is >> target; // target vtx

    queue<Arc> network_arcs;

    while (m--) {
        int u, v; is >> u >> v;
        Arc a; tie(a, ignore) = add_edge(--u, --v, network);
        is >> network[a].capacity;
        network_arcs.push(a);
    }

    return network;

}

/* MAIN */
int main(int argc, char** argv)
{
    Digraph network{read_network(cin)};

    return EXIT_SUCCESS;
}