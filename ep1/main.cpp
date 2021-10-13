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
using namespace boost;

int debug;

typedef adjacency_list<vecS, vecS, directedS> Digraph;
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

/* Digraph Construction */
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

    /* DEBUG
    for_each(vertices(digraph).first, vertices(digraph).second, [&](const auto& vtx) {
        cout << "Arcs that leave " << convert(vtx) << " go to: ";

        for_each(out_edges(vtx, digraph).first, out_edges(vtx, digraph).second, [&](const auto& arc) {
            cout << " " << convert(target(arc, digraph));
        });

        cout << endl;
    });*/

    return digraph;
}

/* 2-SAT Solution */
int main(void)
{
  // Debug
  cin >> debug;
  
  // Digraph Construction
  Digraph phi = read_digraph(cin);
  // cout << num_vertices(phi) << " " << num_edges(phi) << endl; // DEBUG

  return EXIT_SUCCESS;
}