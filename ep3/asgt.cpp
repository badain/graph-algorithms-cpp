#include "asgt.h"

#include <utility>              // for std::get
#include <tuple>
#include <vector>
#include <math.h>              // for log

#define BOOST_ALLOW_DEPRECATED_HEADERS // silence warnings
#include <boost/graph/adjacency_list.hpp>
#include <boost/optional.hpp>

#include "cycle.h"
#include "digraph.h"
#include "potential.h"

/* The code in this template file is all "bogus". It just illustrates
 * how to return answers back to main.cpp. */

/* The following declarations shorten the bogus code below. Feel free
 * to change/drop them. */
using namespace std;
using namespace boost;

// Type Definition: Graph Iterator
typedef boost::graph_traits<Digraph>::vertex_iterator vtx_iterator_type;
typedef boost::graph_traits<Digraph>::edge_iterator edge_iterator_type;
typedef boost::graph_traits<Digraph>::adjacency_iterator adj_iterator_type;
Vertex null_vtx = boost::graph_traits<Digraph>::null_vertex();

/* builds auxiliary market digraph
   process currency exchange rates to allow the negative cycle search
   target execution time = O(n+m)  */
Digraph build_digraph(const Digraph& market)
{
  /* placeholder for NRVO */
  Digraph digraph(num_vertices(market));

  /* replace arc weight (currency ratio) with negative log */
  edge_iterator_type edge_it, edge_end; // iterator edege, last edge
  for (tie(edge_it, edge_end) = edges(market); edge_it != edge_end; ++edge_it) { // edge traversing

    // gets source and target vertex
    Vertex u = source(*edge_it, digraph);
    Vertex v = target(*edge_it, digraph);

    // adds edge to auxiliary
    Arc a;
    std::tie(a, std::ignore) = add_edge(u, v, digraph);

    // process auxiliary to negative log
    digraph[a].cost = -log(market[*edge_it].cost);
    // cout << u+1 << " " << v+1 << " " << digraph[a].cost << endl;

  }

  /* flip some signs in the arc costs below to exercise the many
   * execution pathways */

  /* create arcs (0,1) and (1,0)
  Arc a0, a1;
  std::tie(a0, std::ignore) = add_edge(0, 1, digraph);
  digraph[a0].cost = 11.0;
  std::tie(a1, std::ignore) = add_edge(1, 0, digraph);
  digraph[a1].cost = -17.0; */
 
  return digraph;
}

/* The relaxation technique evaluates if there is a shortest possible distance by updating its property d every time a shortest path is found. */
void relax(Digraph& digraph, Vertex& u, Vertex& v, double& cost, vector<Vertex>& predecessor) {

  if(digraph[v].d > digraph[u].d + cost) { // if there is a shortest path between u and v (if previous distance is larger than new distance)
    digraph[v].d  = digraph[u].d + cost;   // v is tagged with a new shortest distance
    predecessor[v] = u;                    // u is the proper v predecessor in the Shortest Path Tree
  }

}

/* checks for negative-weight cycles in a digraph. uses Bellman-Ford algorithm with an extra vertice (that is, initial distance is 0 not INFINITE) */
std::tuple<bool,
           boost::optional<NegativeCycle>,
           boost::optional<FeasiblePotential>>
has_negative_cycle(Digraph& digraph)
{

  /* initialization */
  // vector<int> discovery(num_vertices(digraph), 0); // discovery weight for each vtx (zero for extra vtx 0)
  vector<Vertex> predecessor(num_vertices(digraph), null_vtx); // predecessor of each vtx
  

  // relax each edges |V| - 1 times
  for(int i = 0; i < int(num_vertices(digraph)-1); i++) {

    // iterate every edge in digraph
    edge_iterator_type edge_it, edge_end; // iterator edege, last edge
    for (tie(edge_it, edge_end) = edges(digraph); edge_it != edge_end; ++edge_it) { // edge traversing
      
      // gets source and target vertex
      Vertex u = source(*edge_it, digraph);
      Vertex v = target(*edge_it, digraph);

      relax(digraph, u, v, digraph[*edge_it].cost, predecessor); // relax edge

    }

  }

  // checks existence of negative-weight cycle
  edge_iterator_type edge_it, edge_end; // iterator edege, last edge
  for (tie(edge_it, edge_end) = edges(digraph); edge_it != edge_end; ++edge_it) { // edge traversing

    // gets source and target vertex
    Vertex u = source(*edge_it, digraph);
    Vertex v = target(*edge_it, digraph);
    // cout << u+1 << " " << v+1 << " " << digraph[*edge_it].cost << endl;

    if(digraph[v].d > digraph[u].d + digraph[*edge_it].cost) {
      return {true, boost::none, boost::none};
    }

  }
  return {false, boost::none, boost::none};

  /* bogus code
  const Arc& a0 = *(out_edges(0, digraph).first);
  const Arc& a1 = *(out_edges(1, digraph).first);

  Walk walk(digraph, 0);
  walk.extend(a0);
  walk.extend(a1); */

  /* Replace `NegativeCycle(walk)` with `boost::none` in the next
   * command to trigger "negative cycle reported but not computed".
   * Comment the whole `return` and uncomment the remaining lines to
   * exercise construction of a feasible potential.

  // encourage RVO
  return {true, NegativeCycle(walk), boost::none}; */

  /* Replace `FeasiblePotential(digraph, y)` with `boost::none` in the
   * next command to trigger "feasible potential reported but not
   * computed".

  // encourage RVO
  vector<double> y(num_vertices(digraph), 0.0);
  return {false, boost::none, FeasiblePotential(digraph, y)}; */
}

Loophole build_loophole(const NegativeCycle& negcycle,
                        const Digraph& aux_digraph,
                        const Digraph& market)
{
  /* bogus code */
  const Arc& b0 = *(out_edges(0, market).first);
  const Arc& b1 = *(out_edges(1, market).first);

  Walk w(market, 0);
  w.extend(b0);
  w.extend(b1);

  // encourage RVO
  return Loophole(w);
}

FeasibleMultiplier build_feasmult(const FeasiblePotential& feaspot,
                                  const Digraph& aux_digraph,
                                  const Digraph& market)
{
  vector<double> z(num_vertices(market), 1.0);

  // encourage RVO
  return FeasibleMultiplier(market, z);
}
