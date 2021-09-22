# graph-algorithms-cpp
 
Codes from the undergratuate Graph Algoriths course @ University of São Paulo (2021)


## BGL: Boost Graph Library
Collection of header files with implementation of commonly used graph algorithms.
It's installed in the repository's root directory. for easy access


### Using windows makefile
`nmake /NOLOGO -f Makefile`

### Input Stream Pipe
`Get-Content .\data_input | .\io_digraph.exe > .\out`

### Compile header
cl.exe /EHsc /W4 /O2 /std:c++20 /I "C:\Program Files\boost\boost_1_77_0" /nologo /exportHeader /headerName:quote digraph.h
### Link Header
cl.exe /EHsc /W4 /O2 /std:c++20 /I "C:\Program Files\boost\boost_1_77_0" /nologo degrees.cpp /link .\digraph.obj