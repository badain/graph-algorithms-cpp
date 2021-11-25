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
  edge_iterator_type edge_it, edge_end; // iterator edge, last edge
  for (tie(edge_it, edge_end) = edges(market); edge_it != edge_end; ++edge_it) { // edge traversing

    // gets source and target vertex
    Vertex u = source(*edge_it, digraph);
    Vertex v = target(*edge_it, digraph);

    // adds edge to auxiliary
    Arc a;
    std::tie(a, std::ignore) = add_edge(u, v, digraph);

    // process auxiliary to negative log
    digraph[a].cost = -log(market[*edge_it].cost);
  }

  return digraph;
}

/* every vtx adjacent to a -INF weighted vtx is also a member of the negative cycle*/
void discover_negative_cycle(Digraph& digraph, Vertex& vtx, vector<Vertex>& predecessor, vector<Vertex>& negativeVertices) {
  Vertex vtx_pi = predecessor[vtx];
  if(digraph[vtx].color == true) {   // if vtx was discovered as negative
    negativeVertices.push_back(vtx); // pushes vtx to negative cycle

    if(digraph[vtx_pi].color == false) { // checks if predecessor is not discovered
      digraph[vtx_pi].color = true;      // discoveres predecessor as negative
      discover_negative_cycle(digraph, vtx_pi, predecessor, negativeVertices); // sends predecessor recursively
    }
  }
}

/* The relaxation technique evaluates if there is a shortest possible distance by updating its property d every time a shortest path is found. */
void relax(Digraph& digraph, Vertex& u, Vertex& v, double& weight, vector<Vertex>& predecessor) {

  if(digraph[v].d > digraph[u].d + weight) { // if there is a shortest path between u and v (if previous distance is larger than new distance)
    digraph[v].d  = digraph[u].d + weight;   // v is tagged with a new shortest distance
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
  vector<Vertex> predecessor(num_vertices(digraph), null_vtx); // predecessor of each vtx
  vector<Vertex> negativeVertices;
  vector<Arc> negativeEdges;

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

    if(digraph[v].color == false && digraph[v].d > digraph[u].d + digraph[*edge_it].cost) {
      // tags v as negative
      // digraph[v].d = -numeric_limits<double>::max(); // tags with -INF (may be unecessary)
      digraph[v].color = true; // tags as discovered

      // adds every v predecessor to the negative cycle
      discover_negative_cycle(digraph, v, predecessor, negativeVertices);
    }

  }

  if(negativeVertices.size() > 0) {
    Walk negativeCycle(digraph, negativeVertices[negativeVertices.size()-1]);
    for (int vtx_it = negativeVertices.size()-1; vtx_it >= 0; vtx_it--) {
      const Arc& a0 = *(out_edges(negativeVertices[vtx_it], digraph).first);
      negativeCycle.extend(a0);
    }
    return {true, NegativeCycle(negativeCycle), boost::none};
  }
  else {
    vtx_iterator_type vtx_it, vtx_end;
    vector<double> y(num_vertices(digraph), 0.0);
    for (tie(vtx_it, vtx_end) = vertices(digraph); vtx_it != vtx_end; ++vtx_it) {
      y[*vtx_it] = digraph[*vtx_it].d;
    }
    return {false, boost::none, FeasiblePotential(digraph, y)};
  }
}

Loophole build_loophole(const NegativeCycle& negcycle,
                        const Digraph& aux_digraph,
                        const Digraph& market)
{
  vector<Arc> negativeArcs = negcycle.get();
  Vertex source_vtx = source(negativeArcs[0], market);

  Walk w(market, source_vtx);
  for (auto arc : negativeArcs) {
    Vertex u = source(arc, aux_digraph);
    Vertex v = target(arc, aux_digraph);

    Arc market_arc;
    tie(market_arc, std::ignore) = edge(u, v, market);

    w.extend(market_arc);
  }

  // encourage RVO
  return Loophole(w);
}

FeasibleMultiplier build_feasmult(const FeasiblePotential& feaspot,
                                  const Digraph& aux_digraph,
                                  const Digraph& market)
{

  // get feaspot for each vtx
  vector<double> aux_feaspot = feaspot.potential();
  vector<double> z(aux_feaspot.size(), 1.0);
  for (int i = 0; i < aux_feaspot.size(); i++) {
    z[i] = exp(-aux_feaspot[i]);
  }

  // encourage RVO
  return FeasibleMultiplier(market, z);
}
