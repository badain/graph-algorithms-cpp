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
  HeadStart(std::vector<int>* d, std::vector<int>* f) {
    discover = *d;
    finish = *f;
  }
  int getDiscoverItem(int x) const {
    return discover[x];
  }
  int getFinishItem(int x) const {
    return finish[x];
  }
  std::vector<int> getDiscover() const {
    return discover;
  }
  std::vector<int> getFinish() const {
    return finish;
  }
private:
  std::vector<int> discover;
  std::vector<int> finish;
};

#endif // #ifndef ARB_H