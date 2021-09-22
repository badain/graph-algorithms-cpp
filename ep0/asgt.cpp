/**********************************************************************
* Description: asgt.cpp
* Dependencies: Boost; asgt.h
*
* Author: Rafael Badain @ University of Sao Paulo
**********************************************************************/
#include "asgt.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;
using namespace std;

Arb read_arb(std::istream& in)
{
  /* reads number of vertices in arborescence */
  typedef typename graph_traits<Arb>::vertices_size_type vtx_size_type; // num of vertices type
  vtx_size_type num_vtx; in >> num_vtx;  // extracts number of vertices
  size_t num_arcs; num_arcs = num_vtx-1; // extracts number of arcs

  /* builds arcs vector */
  vector<pair<int, int>> arcs;             // vector of arcs (int pairs)
  while(num_arcs--) {                      // [num_arcs, 0)
      int u, v; in >> u >> v;              // extracts pair of vtx
      arcs.push_back(make_pair(--u, --v)); // pushes pair to the arcs vector
  }

  /* builds arborescence */
  Arb arb(arcs.begin(), // EdgeIterator = pair<int, int>
          arcs.end(),   // EdgeIterator = pair<int, int>
          num_vtx);     // vtx_size_type

  /* DEBUG
  cout << "num_vtx: " << num_vtx << endl;

  cout << "arcs: " << arcs.size() << endl;
  for (const auto& p : arcs)
    cout << p.first << ", " << p.second << endl;

  cout << "arb: " << endl;
  for_each(vertices(arb).first, vertices(arb).second, [&](const auto& vtx) {
      cout << "Arcs that leave " << vtx+1 << " go to: ";

      for_each(out_edges(vtx, arb).first, out_edges(vtx, arb).second, [&](const auto& arc) {
          cout << " " << target(arc, arb)+1;
      });

      cout << endl;
  });
  */

  return arb;
}

HeadStart preprocess(Arb &arb, const Vertex& root)
{
  return HeadStart(42);
}

bool is_ancestor (const Vertex& u, const Vertex& v, const HeadStart& data)
{
  return false;
}