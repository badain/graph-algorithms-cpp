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
// define guard DIGRAPH_H
#ifndef DIGRAPH_H
#define DIGRAPH_H

// dependencies
#include <iostream>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

// Digraph Type Definition
typedef adjacency_list<vecS, vecS, directedS> Digraph;
// Digraph Input Stream Reading
Digraph read_digraph(istream& in);

#endif // ends define guard DIGRAPH_H