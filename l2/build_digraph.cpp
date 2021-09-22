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

int main (void) {
    /* DIGRAPH BUILDING */
    // Type Definition: Digraph
    typedef adjacency_list<vecS, vecS, directedS> Digraph; //arcs on each vertex, vertice

    // Digraph with no arcs
    int num_vertices = 5;
    Digraph digraph(num_vertices);

    // Add arcs [0, num_vertices)
    add_edge(0, 1, digraph);
    add_edge(0, 2, digraph);
    add_edge(1, 2, digraph);
    add_edge(2, 0, digraph);
    add_edge(3, 1, digraph);
    add_edge(3, 2, digraph);

    /* DIGRAPH ITERATION */
    // Type Definition: Digraph Iterator
    typedef graph_traits<Digraph>::vertex_iterator vtx_iterator_type;
    
    // Traverse
    vtx_iterator_type vtx_it, vtx_end; // iterator, end of the digraph
    
    for (tie(vtx_it, vtx_end) = vertices(digraph); vtx_it != vtx_end; ++vtx_it) {    // tie unwraps the vertices() pair
        typename graph_traits<Digraph>::out_edge_iterator out_edge_it, out_edge_end; // out edge iterator for vtx_it

        cout << "Arc that leave " << *vtx_it + 1 << " go to:"; // vtx iteration
        for (tie(out_edge_it, out_edge_end) = out_edges(*vtx_it, digraph); out_edge_it != out_edge_end; ++out_edge_it) {
            cout << " " << target(*out_edge_it, digraph) + 1;
        }
        cout << endl;
    }
    
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