/**********************************************************************
* Author: Rafael Badain @ University of Sao Paulo
**********************************************************************/

/* Imports */
// STD
#include <iostream>
#include <cmath>
using namespace std;

// BOOST
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/transpose_graph.hpp>
using namespace boost;

int debug;

typedef adjacency_list<vecS, vecS, directedS> Digraph;
typedef graph_traits<Digraph>::vertex_descriptor Vertex;
int num_variables;

/* Helper Function */
/* converts a to a special notation where absolute value is offset  */
/* after the num_variables as the Digraph does not accepts negative indexes */
int convert(int a) {
  if(a < 0) return (abs(a) + num_variables);
  if(a > num_variables) return -(a - num_variables);
  return a;
}
int neg(int a) {
  if(a > num_variables) return (a - num_variables);
  return (a + num_variables);
}

/* Debug Functions */
/* functions to help development */
void print_graph(Digraph &digraph) {
  for_each(vertices(digraph).first, vertices(digraph).second, [&](const auto& vtx) {
    cout << "Arcs that leave " << convert(vtx) << " go to: ";

    for_each(out_edges(vtx, digraph).first, out_edges(vtx, digraph).second, [&](const auto& arc) {
        cout << " " << convert(target(arc, digraph));
    });

    cout << endl;
  });
}
vector<Vertex> get_out_neighbours(Digraph &digraph, Vertex vtx) {
  vector<Vertex> a;
  cout << "get_out_neighbours " << vtx << endl;
  for_each(out_edges(vtx, digraph).first, out_edges(vtx, digraph).second, [&](const auto& arc) {
    a.push_back(target(arc, digraph));
  });
  for (int x : a) cout << x << " ";
  cout << endl;
  return a;
}

/* 2. Finding Strong Connected Components via Kosaraju's */
void assign_scc(Digraph &t_digraph, Vertex vtx, int root, vector<int>* components) {
  (*components)[vtx] = root; // each vertex is assigned the root of its SCC
  
  // Out-neighbour transposed traversal
  for_each(out_edges(vtx, t_digraph).first, out_edges(vtx, t_digraph).second, [&](const auto& arc) {
    Vertex adj = target(arc, t_digraph);
    if((*components)[adj] == 0) assign_scc(t_digraph, adj, root, components);
  });
}

void dfs(Digraph &digraph, Vertex vtx, vector<bool>* visited, vector<Vertex>* inverted_topsort) {
  (*visited)[vtx] = true; // marks vtx as visited
  // Out-neighbour traversal
  for_each(out_edges(vtx, digraph).first, out_edges(vtx, digraph).second, [&](const auto& arc) {
    Vertex adj = target(arc, digraph);
    if((*visited)[adj] == false) dfs(digraph, adj, visited, inverted_topsort);
  });

  (*inverted_topsort).push_back(vtx); // push vtx @ inverted_topsort_array
}

vector<int> scc_search(Digraph &digraph, Digraph &t_digraph) {
  // Vector Initialization
  vector<bool> visited(num_vertices(digraph), false);
  vector<Vertex> inverted_topsort; // topological order inverted
  vector<int> components(num_vertices(digraph), 0); // SCC are respresented by its root vertex. Each vertex from the digraph is assigned the root of its SCC.

  // First Kosaraju's traversal
  auto vp = vertices(digraph);
  for_each(vp.first, vp.second, [&](const auto& vtx) {
    if(vtx == 0) return;
    if(visited[vtx] == false) dfs(digraph, vtx, &visited, &inverted_topsort);
  });

  // Second Kosaraju's traversal
  int r = 1;
  for(int i = (inverted_topsort.size() - 1); i >= 0; i--) {
    if(components[inverted_topsort[i]] == 0) assign_scc(t_digraph, inverted_topsort[i], r, &components);
    r = r + 1;
  }

  if(debug == 1) {
    for (int h : components) cout << h << " ";
    cout << endl;
  }

  /* DEBUG 
  for (int x : visited) cout << x << " ";
  cout << endl;
  for (int y : inverted_topsort) cout << convert(y) << " ";
  cout << endl;
  for (int z : components) cout << convert(z) << " ";
  cout << endl;  */
  
  return components;
}

/* 1. Digraph Construction */
Digraph read_digraph(istream& in){
    
    in >> num_variables; // extracts number of variables = (num_vertices / 2)
    size_t num_clauses; in >> num_clauses; // extracts number of clauses = (num_arcs / 2)

    Digraph digraph(0); // initialize empty digraph

    if(debug == 2) cout << num_variables << " " << (num_clauses*2) << endl; // num_arcs = num_clauses*2

    while(num_clauses--) {       // from num_clauses until 0
        int a, b; in >> a >> b;  // extracts clause (a || b)
        if(a < 0) a = abs(a) + num_variables; // converts neg a and b to special notation where absolute value is offset
        if(b < 0) b = abs(b) + num_variables; // after the num_variables as the Digraph does not accepts negative indexes
        add_edge(neg(a), b, digraph); // adds (!a ~> b) to the Digraph
        add_edge(neg(b), a, digraph); // adds (!b ~> a) to the Digraph
        if(debug == 2) cout << convert(neg(a)) << " " << convert(b) << endl << convert(neg(b)) << " " << convert(a) << endl;
    }

    return digraph;
}

bool validate(vector<int>* components, vector<bool>* solution) {
  for(int i = 1; i <= num_variables; i++) {
    if((*components)[i] == (*components)[i + num_variables]) return false;
    (*solution)[i] = (*components)[i] > (*components)[i + num_variables];
  }
  return true;
}

/* 2-SAT Solution */
int main(void)
{
  // Debug
  cin >> debug;
  
  // Digraph Construction
  Digraph phi = read_digraph(cin);
  Digraph t_phi;
  transpose_graph(phi, t_phi);

  // SCC Search
  vector<int> components = scc_search(phi, t_phi);
  if(debug == 0) {
    vector<bool> solution(num_variables, false);
    bool validation = validate(&components, &solution);
    if(!validation) cout << "NO" << endl;
    else {
      cout << "YES ";
      for (int z : solution) cout << z << " ";
      cout << endl;
    }
  }
  
  /* DEBUG
  cout << "Digraph Phi" << endl;
  print_graph(phi);
  cout << num_vertices(phi) << " " << num_edges(phi) << endl;
  cout << "Digraph Phi Transposed" << endl;
  print_graph(t_phi);
   */

  return EXIT_SUCCESS;
}