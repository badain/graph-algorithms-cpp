/**********************************************************************
* Description: asgt.cpp
* Dependencies: Boost; asgt.h
*
* Author: Rafael Badain @ University of Sao Paulo
**********************************************************************/
#include "asgt.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>

using namespace boost;
using namespace std;

template < typename TimeMap >
class dfs_time_visitor : public default_dfs_visitor
{
    typedef typename property_traits< TimeMap >::value_type T;

public:
    dfs_time_visitor(TimeMap dmap, TimeMap fmap, T& t)
    : m_dtimemap(dmap), m_ftimemap(fmap), m_time(t)
    {
    }
    template < typename Vertex, typename Graph >
    void discover_vertex(Vertex u, const Graph& g) const
    {
        put(m_dtimemap, u, m_time++);
    }
    template < typename Vertex, typename Graph >
    void finish_vertex(Vertex u, const Graph& g) const
    {
        put(m_ftimemap, u, m_time++);
    }
    TimeMap m_dtimemap;
    TimeMap m_ftimemap;
    T& m_time;
};

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
  std::vector< vtx_size_type > dtime(num_vertices(arb));
  std::vector< vtx_size_type > ftime(num_vertices(arb));

  typedef iterator_property_map< std::vector< vtx_size_type >::iterator,
        property_map< Arb, vertex_index_t >::const_type >
        time_pm_type;
  time_pm_type dtime_pm(dtime.begin(), get(vertex_index, arb));
  time_pm_type ftime_pm(ftime.begin(), get(vertex_index, arb));
  vtx_size_type t = 0;
  dfs_time_visitor< time_pm_type > vis(dtime_pm, ftime_pm, t);
  depth_first_search(arb, visitor(vis));

  
  std::vector< vtx_size_type > discover_order(num_vertices(arb));
  integer_range< vtx_size_type > r(0, num_vertices(arb));
  std::copy(r.begin(), r.end(), discover_order.begin());
  std::sort(discover_order.begin(), discover_order.end(),
      indirect_cmp< time_pm_type, std::less< vtx_size_type > >(dtime_pm));
  
  std::vector< vtx_size_type > finish_order(num_vertices(arb));
  std::copy(r.begin(), r.end(), finish_order.begin());
  std::sort(finish_order.begin(), finish_order.end(),
      indirect_cmp< time_pm_type, std::less< vtx_size_type > >(ftime_pm));

  /* DEBUG
  std::cout << "order of discovery: ";
  int i;
  for (i = 0; i < num_vertices(arb); ++i)
      std::cout << discover_order[i]+1 << " ";
  std::cout << std::endl << "order of finish: ";
  for (i = 0; i < num_vertices(arb); ++i)
      std::cout << finish_order[i]+1 << " ";
  std::cout << std::endl;
  */

  return HeadStart(discover_order, finish_order);
}

bool is_ancestor (const Vertex& u, const Vertex& v, const HeadStart& data)
{
  //cout << data.getDiscover().size() << endl;
  //cout << data.getFinish().size() << endl;
  if(u == v) return true;
  std::vector< vtx_size_type > discover = data.getDiscover();
  std::vector< vtx_size_type > finish = data.getFinish();
  int i;
  for (i = 0; i < discover.size(); i++) {
    if(discover[i] == u) break;
    if(discover[i] == v) return false;
  }
  for (i = 0; i < finish.size(); i++) {
    if(finish[i] == u) return false;
    if(finish[i] == v) break;
  }

  return true;
}