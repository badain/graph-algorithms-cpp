// define guard GRAPH_H
#ifndef GRAPH_H
#define GRAPH_H

// dependencies
#include <iostream>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

// namespaces
using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

Graph read_graph(istream& in);

#endif