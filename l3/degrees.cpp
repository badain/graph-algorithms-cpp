/**********************************************************************
* Description:
* Dependencies: Boost; iostream; algorithm
* Usage:
* Positional arguments:
* Optional arguments:
* Post-Condition:
*
* Author: Rafael Badain @ University of Sao Paulo
**********************************************************************/
#include <iostream>
#include <algorithm>
#include "digraph.h"
#define BOOST_ALLOW_DEPRECATED_HEADERS

using namespace std;
using namespace boost;

int main() {
    Digraph digraph = read_digraph(cin); // iostream graph reading

    // graph iteration
    auto vp = vertices(digraph);

    for_each(vp.first, vp.second, [&](const auto& vtx) {
        cout << "The degree of " << vtx  << " is "
             << out_degree(vtx, digraph) << endl;
    });

    return EXIT_SUCCESS;
}