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

void dfs_visit(Arb &arb, Vertex vtx, vector<char>* color, vector<Vertex>* predecessor, int* time, vector<int>* discover, vector<int>* finish) {
  (*color)[vtx] = 'g';
  (*time) += 1;
  (*discover)[vtx] = (*time);

  for_each(out_edges(vtx, arb).first, out_edges(vtx, arb).second, [&](const auto& arc) {
    Vertex adj = target(arc, arb);
    if((*color)[adj] == 'w') {
      (*predecessor)[adj] = vtx;
      dfs_visit(arb, adj, &(*color), &(*predecessor), &(*time), &(*discover), &(*finish));
    }
  });
  
  (*color)[vtx] = 'b';
  (*time) += 1;
  (*finish)[vtx] = (*time);
}

pair<vector<int>, vector<int>> dfs(Arb &arb) {
  // vector initialization
  vector<int> discover(num_vertices(arb));
  vector<int> finish(num_vertices(arb));
  vector<char> color(num_vertices(arb), 'w');
  vector<Vertex> predecessor(num_vertices(arb));
  int time; time = 0;

  /* graph traversing: dfs visiting */
  auto vp = vertices(arb);
  for_each(vp.first, vp.second, [&](const auto& vtx) {
    if(color[vtx] == 'w') dfs_visit(arb, vtx, &color, &predecessor, &time, &discover, &finish);
  });

  /* DEBUG
  for (int x : discover) cout << x << " ";
  cout << endl;
  for (int y : finish) cout << y << " ";
  cout << endl;
  */

  return make_pair(discover,finish);   
}

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
    cout << p.first+1 << ", " << p.second+1 << endl;

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
  pair<vector<int>, vector<int>> discover_finish;
  discover_finish = dfs(arb);
  return HeadStart(discover_finish);
}

bool is_ancestor (const Vertex& u, const Vertex& v, const HeadStart& data)
{
  if(u == v) return true; // trivial case

  vector<int> discover = data.getDiscover();
  vector<int> finish   = data.getFinish();
  
  if(discover[u] > discover[v] || finish[v] > finish[u]) return false; // based on corollary

  return true;
}