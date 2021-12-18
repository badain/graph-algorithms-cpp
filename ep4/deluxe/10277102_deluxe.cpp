/**********************************************************************
* Author: Rafael Badain @ University of Sao Paulo
**********************************************************************/
/* DEPENDENCIES */
#include <cstdlib> // size_t EXIT_SUCCESS EXIT_FAILURE

#include <algorithm> // min
#include <iostream>  // cin cout
#include <limits>    // numeric_limits
#include <tuple>     // tie ignore
#include <queue>     // queue

#define BOOST_ALLOW_DEPRECATED_HEADERS // silence warnings
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

using namespace std;
using namespace boost;

bool DEBUG = false;
bool PRINT = true;

/* DIGRAPH */
struct BundledVertex
{
  int d; // used in BFS DFS
  int f; // used in DFS
  bool color; // used in BFS DFS
  bool visited; // used to obtain unique arcs for Df'
  BundledVertex() : d(0), f(0), color(false), visited(false) {}
};
struct BundledArc
{
  int capacity;
  int flow;
  int order;
  bool phi;
  BundledArc() : capacity(0), flow(0), order(0), phi(true) {}
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
auto read_network(istream& is) {

  struct network_data {
    Digraph network;
    vector<Arc> network_arcs;
    Vertex source;
    Vertex target;
  };

  typename graph_traits<Digraph>::vertices_size_type n; is >> n; // number of vertices

  Digraph network(n);

  size_t m; is >> m; // number of arcs

  Vertex source; is >> source; source -= 1; // source vtx
  Vertex target; is >> target; target -= 1; // target vtx

  network_data data;
  data.network = network;
  data.source = source;
  data.target = target;

  int order = 1; // keeps track of arc ordem for output specifications
  while (m--) {
    int u, v; is >> u >> v;
    Arc a; tie(a, ignore) = add_edge(--u, --v, data.network);
    is >> data.network[a].capacity;
    data.network[a].order = order; order++;
    data.network_arcs.push_back(a);
  }

  return data;

}

/* DFS VISIT */
void dfs_visit(Digraph& digraph, Vertex& u, int& time, vector<Vertex>& predecessor) {

  /* updates vtx u discovery (UNUSED) */
  time += 1;
  digraph[u].d = time;

  // explores u descendents
  adj_iterator_type adj_it, adj_end;
  for (tie(adj_it, adj_end) = adjacent_vertices(u, digraph); adj_it != adj_end; ++adj_it) {
    Vertex v = (*adj_it);
    Arc uv; tie(uv, ignore) = edge(u, v, digraph);
    int uv_residual = (digraph[uv].capacity - digraph[uv].flow);
    if(uv_residual > 0) {
      // if no predecessor has been assigned to v
      if((predecessor[v] == null_vtx)) {
        if(DEBUG) cout << "  parent: " << u+1 << " adj: " << v+1  << endl; // update
        predecessor[v] = u;   // the current vtx is the predecessor for its decendents
        dfs_visit(digraph, v, time, predecessor);
      }
      // if a predecessor has been assigned to v, checks if a replacement is viable
      else if(predecessor[v] != null_vtx && predecessor[v] != u) {
        // evaluates residual capacities
        Arc prev; tie(prev, ignore) = edge(predecessor[v], v, digraph);
        int prev_residual = (digraph[prev].capacity - digraph[prev].flow); // arc of the previously assigned predecessor

        // (st-flow g) definition requires the largest residual capacities
        if(uv_residual > prev_residual) {
          predecessor[v] = u;   // the current vtx is the predecessor for its decendents
          if(DEBUG) cout << "- parent: " << u+1 << " adj: " << v+1 << endl; // replace
          dfs_visit(digraph, v, time, predecessor);
        }
        else if(DEBUG) cout << "x parent: " << u+1 << " adj: " << v+1 << endl; // reject
      }
    }
    else if(DEBUG) cout << "o parent: " << u+1 << " adj: " << v+1 << endl; // residual reject
  }

  /* updates vtx u finish (UNUSED) */
  time += 1;
  digraph[u].f = time;
  digraph[u].color = true;
}

/* DFS */
auto dfs(Digraph& digraph, Vertex& source, Vertex& target) {

  struct st_flow_g_data {
    vector<Vertex> pi;
    bool status;
  };
  st_flow_g_data st_flow_g;

  // initialization
  vtx_iterator_type vtx_it, vtx_end;
  for (tie(vtx_it, vtx_end) = vertices(digraph); vtx_it != vtx_end; ++vtx_it) {
    digraph[*vtx_it].d = 0;
    digraph[*vtx_it].color = false;
  }
  vector<Vertex> predecessor(num_vertices(digraph), null_vtx);
  int time = 0;

  // dfs visits each vertex
  for (tie(vtx_it, vtx_end) = vertices(digraph); vtx_it != vtx_end; ++vtx_it) {
    if(DEBUG) cout << "dfs_level: " << (*vtx_it)+1 << endl;
    if(!digraph[*vtx_it].color) {
      Vertex u = (*vtx_it);
      dfs_visit(digraph, u, time, predecessor);
    }
  }

  // checks st-flow validity
  for (Vertex v = target; v != source; v = predecessor[v]) {
    if(v == null_vtx) { // before reaching the source there is a null_vtx, thus there is no st-flow
      st_flow_g.status = false;
      return st_flow_g;
    }
  }

  st_flow_g.status = true;
  st_flow_g.pi = predecessor;

  if(DEBUG) {
    cout << "st-flow g: ";
    for(auto i : predecessor) cout << i+1 << " ";
    cout << endl;
  }

  return st_flow_g;
}

/* RESIDUAL DF' GENERATION */
void generate_df_line(Digraph& digraph, Digraph& df_line, Vertex& current, vector<Vertex>& pi, vector<vector<Vertex>>& predecessor){
  
  if(!digraph[current].visited && pi.size() > 0) { // if current vector arcs were not extracted and is not the source

    if(DEBUG) {
      cout << current+1 << " |";
      for(auto pred : pi) cout << " " << pred+1;
      cout << endl;
    }

    digraph[current].visited = true; // avoids repetitions
    for(auto pred : pi) { // for each predecessor of the current vtx
      Arc a_original; tie(a_original, ignore) = edge(pred, current, digraph);
      Arc a; tie(a, ignore) = add_edge(pred, current, df_line); // adds edge from predecessor to current vtx
      df_line[a].capacity = digraph[a_original].capacity; // stores arc capacity
      generate_df_line(digraph, df_line, pred, predecessor[pred], predecessor); // ads the predecessor arcs for each predecessor (if unvisited)
    }
  }

}

/* BFS */
auto bfs(Digraph& digraph, Vertex& source, Vertex& target, vector<vector<Vertex>>& predecessor) {

  struct df_line_data {
    Digraph network;
    bool status;
  };
  df_line_data df_line;

  // initialization
  vtx_iterator_type vtx_it, vtx_end;
  for (tie(vtx_it, vtx_end) = vertices(digraph); vtx_it != vtx_end; ++vtx_it) {
    digraph[*vtx_it].color = false;
    digraph[*vtx_it].d = 0;
  }

  // start procedure by tagging source
  digraph[source].color = true;
  queue<Vertex> bfs_queue; bfs_queue.push(source); // FIFO

  // bfs traversal
  while(!bfs_queue.empty()) {
    // removes u from traversing queue
    Vertex u = bfs_queue.front();
    bfs_queue.pop();
    if(DEBUG) cout << "bfs_queue: " << u+1 << endl;

    // visits u decendents
    adj_iterator_type adj_it, adj_end;
    for (tie(adj_it, adj_end) = adjacent_vertices(u, digraph); adj_it != adj_end; ++adj_it) {
      if(DEBUG) cout << "  adj: " << (*adj_it)+1 << endl;
      // visits unvisited decendent
      if(!digraph[*adj_it].color) {
        digraph[*adj_it].color = true;         // marks as visited
        digraph[*adj_it].d = digraph[u].d + 1; // BF-tree: distance from source
        predecessor[*adj_it].push_back(u);     // BF-tree: path to source

        bfs_queue.push(*adj_it); // schedule visit to decendents of decendent
      }
      // if a descendent was already visited (has another predecessor), also stores the current vtx IF they are in the same distance
      else if(predecessor[*adj_it].size() > 0 && digraph[u].d == digraph[predecessor[*adj_it][0]].d) {
        predecessor[*adj_it].push_back(u); // BF-tree: path to source
      }

    }
  }

  if(predecessor[target].size() > 0) { // calculates residual digraph Df'
    if(DEBUG) cout << "residual digraph:" << endl;
    generate_df_line(digraph, df_line.network, target, predecessor[target], predecessor); // starts generation from target
    df_line.status = true;
    return df_line;
  }

  df_line.status = false;
  return df_line; // no st-path was found

}

/* EDMONDS */

/* MAIN */
int main(int argc, char** argv)
{
    auto data = read_network(cin); // data.network data.network_arcs data.source data.target
    vector<Vertex> pi; // list of predecessors of a vertex pi
    vector<vector<Vertex>> predecessor(num_vertices(data.network), pi); // predecessors of each vtx of the network

    // generate residual network graph
    vector<Arc> network_arcs_b; // positive forward arcs, negative backwards arcs
    size_t order = 1; 
    for(Arc arc : data.network_arcs) {
      // gets original arc vertices
      Vertex u = source(arc, data.network);
      Vertex v = target(arc, data.network);

      // backward arcs
      Arc b; tie(b, ignore) = add_edge(v, u, data.network);
      data.network[b].order = order; // arc order
      data.network[b].phi = false;   // arc direction
      network_arcs_b.push_back(b); // tracks arc order

      order++;
    }

    // generates residual df' neetwork digraph
    auto df_line = bfs(data.network, data.source, data.target, predecessor);

    // calculates maximal feasible (st-flow) g
    auto st_flow_g = dfs(df_line.network, data.source, data.target);

    // evaluates delG
    int min_del_g = numeric_limits<int>::max();
    for (Vertex v = data.target; v != data.source; v = st_flow_g.pi[v]) { // traverses st-path
      Arc uv; bool uv_exists; tie(uv, uv_exists) = edge(st_flow_g.pi[v], v, df_line.network);
      Arc vu; bool vu_exists; tie(vu, vu_exists) = edge(v, st_flow_g.pi[v], df_line.network);

      // flow g(b) evaluation: by definition, if b is not in df', then g(b) = 0
      int uv_residual = (uv_exists) ? (df_line.network[uv].capacity - df_line.network[uv].flow) : 0;
      int vu_residual = (vu_exists) ? (df_line.network[vu].capacity - df_line.network[vu].flow) : 0;

      int del_g = uv_residual - vu_residual;
      min_del_g = min(min_del_g, del_g);
    }

    // updates flow along the st-path g
    vector<int> path_order;
    for (Vertex v = data.target; v != data.source; v = st_flow_g.pi[v]) { // for each edge in the augmenting path
      Arc uv; bool uv_exists; tie(uv, uv_exists) = edge(st_flow_g.pi[v], v, df_line.network);
      Arc vu; bool vu_exists; tie(vu, vu_exists) = edge(v, st_flow_g.pi[v], df_line.network);

      // updates flow @ df'
      if(uv_exists) df_line.network[uv].flow = df_line.network[uv].flow + min_del_g;
      if(vu_exists) df_line.network[vu].flow = df_line.network[vu].flow - min_del_g;

    }

    if(DEBUG) {
      arc_iterator_type arc_it, arc_end;
      for (tie(arc_it, arc_end) = edges(df_line.network); arc_it != arc_end; ++arc_it) {
        cout << (*arc_it) << " " << df_line.network[(*arc_it)].capacity << " " << df_line.network[(*arc_it)].flow << endl;
      }
    }

    if(DEBUG) {
      vtx_iterator_type vtx_it, vtx_end;
      for (tie(vtx_it, vtx_end) = vertices(df_line.network); vtx_it != vtx_end; ++vtx_it) {
        cout << (*vtx_it)+1 << " " << df_line.network[*vtx_it].color << " " << df_line.network[*vtx_it].d << " " << ((*vtx_it) == data.source) << " " << ((*vtx_it) == data.target) << endl;
      }
    }

    return EXIT_SUCCESS;
}