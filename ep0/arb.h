/* the definition of HeadStart may be changed in this file; the rest
 * MUST NOT be changed
 */

#ifndef ARB_H
#define ARB_H

#define BOOST_ALLOW_DEPRECATED_HEADERS // silence warnings
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::directedS> Arb;
typedef boost::graph_traits<Arb>::vertex_descriptor Vertex;

/* Students must adapt this class for their needs; the innards of
   these objects are only accessed by the student's code */
class HeadStart {
public:
  HeadStart(std::pair<std::vector<int>, std::vector<int>> d_f) {
    discover_order = d_f.first;
    finish_order = d_f.second;
  }
  std::vector<int> getDiscover() const {
    return discover_order;
  }
  std::vector<int> getFinish() const {
    return finish_order;
  }
private:
  std::vector<int> discover_order;
  std::vector<int> finish_order;
};

#endif // #ifndef ARB_H