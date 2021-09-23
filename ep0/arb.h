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
typedef typename boost::graph_traits<Arb>::vertices_size_type vtx_size_type; // num of vertices type

/* Students must adapt this class for their needs; the innards of
   these objects are only accessed by the student's code */
class HeadStart {
public:
  HeadStart(std::vector< vtx_size_type > a, std::vector< vtx_size_type > b) {
    discover_order = a;
    finish_order = b;
  }
  std::vector< vtx_size_type > getDiscover() const {
    return discover_order;
  }
  std::vector< vtx_size_type > getFinish() const {
    return finish_order;
  }
private:
  std::vector< vtx_size_type > discover_order;
  std::vector< vtx_size_type > finish_order;
};

#endif // #ifndef ARB_H