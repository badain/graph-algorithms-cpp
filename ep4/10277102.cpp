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
  double flow;
  BundledArc() : capacity(0.0), flow(0.0) {}
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

bool DEBUG = false;

/* INPUT */
auto read_network(istream& is) {

    struct network_data {
        Digraph network;
        queue<Arc> network_arcs;
        Vertex source;
        Vertex target;
    };

    typename graph_traits<Digraph>::vertices_size_type n; is >> n; // number of vertices

    Digraph network(n);

    size_t m; is >> m; // number of arcs

    Vertex source; is >> source; source -= 1;// source vtx
    Vertex target; is >> target; target -= 1;// target vtx

    queue<Arc> network_arcs;

    while (m--) {
        int u, v; is >> u >> v;
        Arc a; tie(a, ignore) = add_edge(--u, --v, network);
        is >> network[a].capacity;
        network_arcs.push(a);
    }

    return network_data{network, network_arcs, source, target};

}

/* BFS */
void bfs(Digraph& digraph, Vertex& source, Vertex& target) {

  // initialization
  vtx_iterator_type vtx_it, vtx_end;
  for (tie(vtx_it, vtx_end) = vertices(digraph); vtx_it != vtx_end; ++vtx_it) {
    digraph[*vtx_it].color = false;
    digraph[*vtx_it].d = 0;
  }
  vector<Vertex> predecessor(num_vertices(digraph), null_vtx); // predecessor reset

  // start procedure by tagging source
  digraph[source].color = true;
  queue<Vertex> bfs_queue; bfs_queue.push(source); // FIFO

  // bfs traversal
  while(!bfs_queue.empty()) {
    // removes u from traversing queue
    Vertex u = bfs_queue.front();
    bfs_queue.pop();

    // visits u decendents
    adj_iterator_type adj_it, adj_end;
    for (tie(adj_it, adj_end) = adjacent_vertices(u, digraph); adj_it != adj_end; ++adj_it) {
      if(!digraph[*adj_it].color) {
        digraph[*adj_it].color = true;
        bfs_queue.push(*adj_it);
        digraph[*adj_it].d = digraph[u].d + 1;
        predecessor[*adj_it] = u;
      }
    }
  
  }

}

/* EDMONDS */

/* MAIN */
int main(int argc, char** argv)
{
    auto data = read_network(cin); // data.network data.network_arcs data.source data.target

    bfs(data.network, data.source, data.target);

    /* DEBUG */
    if(DEBUG) {
    vtx_iterator_type vtx_it, vtx_end;
    for (tie(vtx_it, vtx_end) = vertices(data.network); vtx_it != vtx_end; ++vtx_it) {
      cout << (*vtx_it)+1 << " " << data.network[*vtx_it].color << " " << data.network[*vtx_it].d << " " << ((*vtx_it) == data.source) << " " << ((*vtx_it) == data.target) << endl;
    }
    }

    return EXIT_SUCCESS;
}