#include "asgt.h"

#include <stack>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace boost;

// Type Definition: Graph Iterator
typedef boost::graph_traits<Graph>::vertex_iterator vtx_iterator_type;
typedef boost::graph_traits<Graph>::adjacency_iterator adj_iterator_type;
Vertex null_vtx = graph_traits<Graph>::null_vertex();

void DFS_bcc(Graph &g, Vertex vtx, vector<bool>* visited, vector<int>* lowpoint, vector<int>* discovery, vector<Vertex>* parent, stack<Edge>* bcc_stack, int* time, int* bcc_group) {
  
  (*visited)[vtx] = true; // marks vtx as visited
  (*time) += 1; // increments time
  (*discovery)[vtx] = (*lowpoint)[vtx] = (*time); // assigns time to vtx discovery and lowpoint

  int child_num = 0; // child count

  // iterates through adjacents of vtx
  for_each(adjacent_vertices(vtx, g).first, adjacent_vertices(vtx, g).second, [&](const auto& adj) {
    Edge e; bool found;
    tie(e, found) = edge(vtx, adj, g);
    if((*visited)[adj] == false) { // if adjacent vtx wasnt visited
      child_num += 1; // increments child count of vtx
      (*parent)[adj] = vtx; // stores vtx as adj parent
      (*bcc_stack).push(e); // pushes (vtx, adj) to stack

      DFS_bcc(g, adj, visited, lowpoint, discovery, parent, bcc_stack, time, bcc_group); // traverses child adjacents

      (*lowpoint)[vtx] = min((*lowpoint)[vtx], (*lowpoint)[adj]); // lowest depth between descendents

      // checks for articulation point
      // a nonroot vtx is a articulation point if and only if adj of vtx is such that lowpoint(adj) ≥ depth(vtx)
      if( ((*parent)[vtx] != null_vtx) && ((*lowpoint)[adj] >= (*discovery)[vtx]) ) {
        while ((*bcc_stack).top() != e) {
          g[(*bcc_stack).top()].bcc = (*bcc_group);
          (*bcc_stack).pop();
        }
        g[(*bcc_stack).top()].bcc = (*bcc_group);
        (*bcc_stack).pop();
        (*bcc_group) += 1;
      }
      // if a root vtx has one or more child is a articulation point
      if( ((*parent)[vtx] == null_vtx) && (child_num > 0) ) {
        while ((*bcc_stack).top() != e) {
          g[(*bcc_stack).top()].bcc = (*bcc_group);
          (*bcc_stack).pop();
        }
        g[(*bcc_stack).top()].bcc = (*bcc_group);
        (*bcc_stack).pop();
        (*bcc_group) += 1;
      }

    }
    // if its not a articulation point
    else if( ((*parent)[vtx] != adj) && ((*discovery)[adj] < (*discovery)[vtx]) ) {
      (*bcc_stack).push(e); // pushes (vtx, adj) to stack
      (*lowpoint)[vtx] = min((*lowpoint)[vtx], (*discovery)[adj]); // lowest depth between descendents
    }

  });

}

void compute_bcc (Graph &g, bool fill_cutvxs, bool fill_bridges)
{
  vector<bool> visited(num_vertices(g), false); // discovery status for each vtx
  vector<int>  lowpoint(num_vertices(g), numeric_limits<int>::max()); // lowest depth of neighbors of all descendants of each vtx in the DFS tree
  vector<int>  discovery(num_vertices(g), 0); // discovery time for each vtx
  vector<Vertex> parent(num_vertices(g), null_vtx); // parent of each vtx
  stack<Edge> bcc_stack; // stack of biconnected components
  int time = 0; // current discovery
  int bcc_group = 1;

  vtx_iterator_type vtx_it, vtx_end; // iterator vtx, last vtx
  for (tie(vtx_it, vtx_end) = vertices(g); vtx_it != vtx_end; ++vtx_it) { // traverse graph
    if (visited[*vtx_it] == false) {
      DFS_bcc(g, *vtx_it, &visited, &lowpoint, &discovery, &parent, &bcc_stack, &time, &bcc_group);
      while (!bcc_stack.empty()) {
        g[bcc_stack.top()].bcc = bcc_group;
        bcc_stack.pop();
      }
    }
  }
}
