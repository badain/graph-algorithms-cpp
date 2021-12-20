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
#include <map>       // map

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
  vector<int> groups; // flow groups
  BundledArc() : capacity(0), flow(0), order(0), phi(true), groups(vector<int>()) {}
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
int get_residual(Digraph& digraph, Arc& arc, vector<int>& group_flow) {
  if(digraph[arc].groups.size() > 0) { // if uv is assigned to a group = there is flow through uv
    int flow_uv = 0;
    for(auto g : digraph[arc].groups) flow_uv += group_flow[g];
    return digraph[arc].capacity - flow_uv;
  }
  else return digraph[arc].capacity; // there is no flow through uv
}

int get_flow(Digraph& digraph, Arc& arc, vector<int>& group_flow) {
  if(digraph[arc].groups.size() > 0) {
    int flow_arc = 0;
    for(auto g : digraph[arc].groups) flow_arc += group_flow[g];
    return flow_arc;
  }
  else return 0;
}

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
void dfs_visit(Digraph& digraph, Digraph& dhat, Vertex& u, int& time, vector<Vertex>& predecessor, int& group, Vertex& target, Vertex& source, vector<int>& group_flow, map<int, vector<int>>& resource_usage, vector<int>& target_groups) {

  /* updates vtx u discovery (UNUSED) */
  time += 1;
  digraph[u].d = time;

  // sort u descendents by arc residual capacity
  vector<Vertex> descendents;
  adj_iterator_type adj_it, adj_end;
  for (tie(adj_it, adj_end) = adjacent_vertices(u, digraph); adj_it != adj_end; ++adj_it) descendents.push_back(*adj_it);
  std::sort(descendents.begin(), descendents.end(),
    [&](int a, int b) -> bool {
      Arc ua; tie(ua, ignore) = edge(u, a, digraph);
      Arc ub; tie(ub, ignore) = edge(u, b, digraph);
      return digraph[ua].capacity > digraph[ub].capacity;
    });

  // group tracking
  if((descendents.size() > 1) && (u != source)) {
    group += 1; // switch flow group @ flow segmentation
    group_flow.push_back(-1);
  }

  // explores u descendents
  for(auto v : descendents) {
    Arc uv; tie(uv, ignore) = edge(u, v, digraph);
    Arc uv_hat; tie(uv_hat, ignore) = edge(u, v, dhat);
    if(digraph[uv].capacity > 0) {
      if(DEBUG) cout << group << " " << uv << " " << digraph[uv].capacity;

      // uv residual capacity
      int residual_uv = 0;
      if(digraph[uv].groups.size() > 0) { // if uv is assigned to a group = there is flow through uv
        int flow_uv = 0;
        for(auto g : digraph[uv].groups) flow_uv += group_flow[g];
        residual_uv = digraph[uv].capacity - flow_uv;
      }
      else residual_uv = digraph[uv].capacity; // there is no flow through uv
      if(DEBUG) cout << " " << residual_uv << endl;

      // armazena group flow
      if(group_flow[group] == -1) group_flow[group] = residual_uv;  // group flow is empty
      else group_flow[group] = min(group_flow[group], residual_uv); // there is incoming flow

      // atribui group para current arc uv
      digraph[uv].groups.push_back(group);
      dhat[uv_hat].groups.push_back(group);

      // atualiza group available resources
      if(u != source) {
        Arc prev_arc; tie(prev_arc, ignore) = edge(predecessor[u], u, digraph);

        // evaluates group switch
        vector<int> diff;
        set_difference(digraph[prev_arc].groups.begin(), digraph[prev_arc].groups.end(), digraph[uv].groups.begin(), digraph[uv].groups.end(),
        inserter(diff, diff.begin()));
        
        for(auto g : diff) {
          resource_usage[g].push_back(group);
        }
      }

      if(v == target){ // switch flow group @ flow end
        target_groups.push_back(group); // maybe you should save the arcs instead
        group += 1;
        group_flow.push_back(-1);
      }
      else { // dfs recursion
        predecessor[v] = u;
        dfs_visit(digraph, dhat, v, time, predecessor, group, target, source, group_flow, resource_usage, target_groups);
      } 
    }
  }

  /* updates vtx u finish (UNUSED) */
  time += 1;
  digraph[u].f = time;
  digraph[u].color = true;
}

/* DFS */
auto dfs(Digraph& digraph, Digraph& dhat, Vertex& source, Vertex& target, vector<int>& group_flow, int& group) {
  if(DEBUG) cout << endl << "DFS" << endl;

  struct st_flow_g_data {
    vector<int> group_flow;
    int update_flow;
    bool status;
  };
  st_flow_g_data st_flow_g;

  // initialization
  map<int, vector<int>> resource_usage; // tracks groups that are feeding resources to of other groups
  vector<int> target_groups;
  vtx_iterator_type vtx_it, vtx_end;
  for (tie(vtx_it, vtx_end) = vertices(digraph); vtx_it != vtx_end; ++vtx_it) {
    digraph[*vtx_it].d = 0;
    digraph[*vtx_it].color = false;
  }
  vector<Vertex> predecessor(num_vertices(digraph), null_vtx);
  int time = 0;

  dfs_visit(digraph, dhat, source, time, predecessor, group, target, source, group_flow, resource_usage, target_groups);

  if(DEBUG){
    cout << endl << "group_flow: " << endl;
    for(auto flow : group_flow) cout << flow << " ";
    cout << endl;

    cout << endl << "resource_usage: " << endl;
    for(const auto& elem : resource_usage) {
      cout << elem.first << " | ";
      for(auto i : elem.second) cout << i << " ";
      cout << endl;
    }
  }

  // updates flow groups based on resource usage
  for (auto it = resource_usage.rbegin(); it != resource_usage.rend(); ++it) {
    int used_flow = 0; for(auto using_group : (*it).second) used_flow += group_flow[using_group]; // the flow of a parent group is given by the flow used by its child
    group_flow[(*it).first] = used_flow; // parent flow group is retractively updated
  }

  int update_flow = 0;
  for(auto g : target_groups) update_flow += group_flow[g];

  if(DEBUG){
    cout << endl << "updated_group_flow: " << endl;
    for(auto flow : group_flow) cout << flow << " ";
    cout << endl;

    cout << endl << "target_groups: " << endl;
    for(auto g : target_groups) cout << g << " ";
    cout << endl;

    cout << endl << "update_flow: " << update_flow << endl;
    cout << endl;
    arc_iterator_type adj_it, adj_end;
    for (tie(adj_it, adj_end) = edges(digraph); adj_it != adj_end; ++adj_it){
      cout << (*adj_it) << " ";
      for(auto a : digraph[*adj_it].groups) cout << a << " ";
      cout << endl;
    }
    cout << endl;
    cout << "last group: " << group << endl << endl;
  }

  st_flow_g.status = true;
  st_flow_g.group_flow = group_flow; // group information is encoded @ arc (decodes group into flow)
  st_flow_g.update_flow = update_flow; // total flow updated in this run

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
      generate_df_line(digraph, df_line, pred, predecessor[pred], predecessor); // adds the predecessor arcs for each predecessor (if unvisited)
    }
  }

}

/* BFS */
auto bfs(Digraph& digraph, Vertex& source, Vertex& target, vector<vector<Vertex>>& predecessor, vector<int>& group_flow) {

  struct df_line_data {
    Digraph network;
    bool status;
  };
  df_line_data df_line;

  // initialization
  vtx_iterator_type vtx_it, vtx_end;
  for (tie(vtx_it, vtx_end) = vertices(digraph); vtx_it != vtx_end; ++vtx_it) {
    digraph[*vtx_it].visited = false;
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
      // visits unvisited decendent w/ available residual capacity
      Arc uv; tie(uv, ignore) = edge(u, (*adj_it), digraph);
      int residual_uv = 0;
      if(digraph[uv].groups.size() > 0) { // if uv is assigned to a group = there is flow through uv
        int flow_uv = 0;
        for(auto g : digraph[uv].groups) flow_uv += group_flow[g];
        residual_uv = digraph[uv].capacity - flow_uv;
      }
      else residual_uv = digraph[uv].capacity; // there is no flow through uv
      if(!digraph[*adj_it].color && (residual_uv > 0)) { // minimum residual capacity > 0 garantees residual digraph consistency
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

    // generate residual network graph D^
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

    // initialization
    int group = 0; // group tracking
    vector<int> group_flow; group_flow.push_back(-1); // flow in a flow group (group to flow)
    vector<Vertex> pi; // list of predecessors of a vertex pi
    int max_flow = 0;

    // flow augmentation
    while(true) {
      vector<vector<Vertex>> predecessor(num_vertices(data.network), pi); // predecessors of each vtx of the network
      // generates residual df' network digraph from D^
      auto df_line = bfs(data.network, data.source, data.target, predecessor, group_flow);
      if(!df_line.status) break;

      // calculates maximal feasible (st-flow) g
      auto st_flow_g = dfs(df_line.network, data.network, data.source, data.target, group_flow, group);

      max_flow += st_flow_g.update_flow;

      if(PRINT) {
        cout << "0" << endl;
        // residual capacities
        for (size_t i = 0; i < data.network_arcs.size(); ++i) {
          cout << get_residual(data.network, data.network_arcs[i], group_flow) << " " // cf, g +
               << get_flow(data.network, data.network_arcs[i], group_flow) << " "; // cf, g +
          cout << get_residual(data.network, network_arcs_b[i], group_flow) << " "   // cf, g -
               << get_flow(data.network, network_arcs_b[i], group_flow) << endl;   // cf, g -
        }  
      }
    }

    if(DEBUG) cout << "max_flow: " << max_flow << endl;

    if(PRINT) {
      cout << "1" << endl;
      // residual capacities
      for (size_t i = 0; i < data.network_arcs.size(); ++i) {
        cout << get_residual(data.network, data.network_arcs[i], group_flow) << " "
             << get_residual(data.network, network_arcs_b[i], group_flow) << endl;
      }
      // val(f)
      cout << max_flow << " ";
      // source cut
      vector<Vertex> cut; vtx_iterator_type vtx_it, vtx_end;
      for (tie(vtx_it, vtx_end) = vertices(data.network); vtx_it != vtx_end; ++vtx_it) {
        if(data.network[*vtx_it].color) cut.push_back((*vtx_it)+1);
      }
      cout << cut.size();
      for (auto i : cut) cout << " " << i;
      cout << endl;
    }

    /* 
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
    */

    return EXIT_SUCCESS;
}