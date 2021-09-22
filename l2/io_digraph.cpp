/**********************************************************************
* Description: 
* Dependencies:
* Usage:
* Positional arguments:
* Optional arguments:
* Post-Condition:
*
* Author: Rafael Badain @ University of Sao Paulo
**********************************************************************/
// STD
#include <iostream>
#include <tuple>
#include <array>

// BOOST
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

// Type Definition: Digraph
typedef adjacency_list<vecS, vecS, directedS> Digraph;

Digraph read_digraph(istream& in){
    typedef typename graph_traits<Digraph>::vertices_size_type vtx_size_type; // num of vertices type
    vtx_size_type num_vtx; in >> num_vtx; // extracts number of vertices
    size_t num_arcs; in >> num_arcs;      // extracts number of arcs

    vector<pair<int, int>> arcs; // vector of pairs (arcs)
    while(num_arcs--) {          // from num_arcs until 0
        int u, v; in >> u >> v;  // extracts pair of vtx
        arcs.push_back(make_pair(--u, --v)); // pushes pair to the arcs array
    }

    return Digraph(arcs.begin(), arcs.end(), num_vtx);
}

int main (void) {
    /* DIGRAPH BUILDING */
    Digraph digraph = read_digraph(cin);

    /* DIGRAPH ITERATION */
    // Type Definition: Digraph Iterator
    typedef graph_traits<Digraph>::vertex_iterator vtx_iterator_type;
    
    // Traverse
    // automatically iterates between an array (first iterator, last iterator, function to be applied)
    for_each(vertices(digraph).first, vertices(digraph).second, [&](const auto& vtx) {
        cout << "Arcs that leave " << vtx+1 << " go to: ";

        for_each(out_edges(vtx, digraph).first, out_edges(vtx, digraph).second, [&](const auto& arc) {
            cout << " " << target(arc, digraph) + 1;
        });

        cout << endl;
    });

    return EXIT_SUCCESS;
}