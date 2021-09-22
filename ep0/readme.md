```
cl.exe /EHsc /W4 /O2 /std:c++20 /I "C:\Program Files\boost\boost_1_77_0" /nologo /exportHeader /headerName:quote arb.h
cl.exe /EHsc /W4 /O2 /std:c++20 /I "C:\Program Files\boost\boost_1_77_0" /nologo /exportHeader /headerName:quote asgt.h
cl.exe /EHsc /W4 /O2 /std:c++20 /I "C:\Program Files\boost\boost_1_77_0" /nologo asgt.cpp
cl.exe /EHsc /W4 /O2 /std:c++20 /I "C:\Program Files\boost\boost_1_77_0" /nologo main.cpp /link .\asgt.obj
Get-Content .\tests\in01 | .\main.exe > out
```