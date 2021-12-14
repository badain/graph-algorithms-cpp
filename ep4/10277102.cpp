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
  double weight;
  BundledArc() : weight(0.0) {}
};

typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::directedS,
                              BundledVertex,
                              BundledArc> Digraph;
typedef boost::graph_traits<Digraph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Digraph>::edge_descriptor Arc;