/**********************************************************************
* Description: utility methods for digraph processing
* Dependencies: Boost; iostream
* Usage:
* Positional arguments:
* Optional arguments:
* Post-Condition:
*
* Author: Rafael Badain @ University of Sao Paulo
**********************************************************************/
#define BOOST_ALLOW_DEPRECATED_HEADERS // supress warnings
#include "digraph.h"

using namespace boost;
using namespace std;

Digraph read_digraph(istream& in){
    /* input stream */
    // num of vertices
    typename graph_traits<Digraph>::vertices_size_type num_vtx;
    in >> num_vtx;
    // num of arcs
    size_t num_arcs;
    in >> num_arcs;

    /* graph building */
    Digraph digraph(num_vtx); // zero index
    while (num_arcs--) {
        int u, v;
        in >> u >> v;
        add_edge(u--, v--, digraph); // zero index (--)
    }

    return digraph;
}