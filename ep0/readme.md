```
cl.exe /EHsc /W4 /O2 /std:c++20 /I "C:\Program Files\boost\boost_1_77_0" /nologo /exportHeader /headerName:quote arb.h
cl.exe /EHsc /W4 /O2 /std:c++20 /I "C:\Program Files\boost\boost_1_77_0" /nologo /exportHeader /headerName:quote asgt.h
cl.exe /EHsc /W4 /O2 /std:c++20 /I "C:\Program Files\boost\boost_1_77_0" /nologo asgt.cpp
cl.exe /EHsc /W4 /O2 /std:c++20 /I "C:\Program Files\boost\boost_1_77_0" /nologo main.cpp /link .\asgt.obj
Get-Content .\tests\in01 | .\main.exe > out
```

```
#include <boost/graph/topological_sort.hpp>
  /* getting root by topological sort: since every arborescence is a tree
  typedef boost::graph_traits<Arb>::vertex_descriptor Vertex;
  typedef std::vector< Vertex > container;

  container sort;
  topological_sort(arb, std::back_inserter(sort));
  */
  /* debug
  cout << "A topological ordering: ";
  for ( container::reverse_iterator ii=c.rbegin(); ii!=c.rend(); ++ii)
    cout << *ii + 1 << " ";
  cout << endl;

  cout << "root: " << *sort.rbegin() << endl;
  */
```

./main < ./tests/in08 > sol08
diff ./sol08 ./tests/sol08
tar -czvf 10277102.tar.gz asgt.cpp arb.h

01
order of discovery: 1 2 3 4 5 
order of finish: 3 4 2 5 1 

02
order of discovery: 1 2 4 3 6 5 
order of finish: 3 4 5 6 2 1 

03
order of discovery: 1 2 5 7 4 9 8 6 3 10 
order of finish: 7 5 2 8 9 4 3 6 10 1 

04
order of discovery: 1 2 8 6 7 5 3 4 9 10 
order of finish: 6 7 8 2 3 4 5 10 9 1 