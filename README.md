##*Procedural map generation*

-----
**CMake Build instructions**

-----
  - Out-of-source build :
 ```
git clone git@github.com:LucasGandel/ProceduralMap.git && \
mkdir ProceduralMap-build && cd ProceduralMap-build  && \
cmake ../ProceduralMap  && \
make -j
 
 ```
  - QtCreator :
 ```
git clone git@github.com:LucasGandel/ProceduralMap.git  && \
mkdir ProceduralMap-build && cd ProceduralMap-build  && \
qtcreator ../ProceduralMap
 
 ```
  *Then configure project using QtCreator API* : 
   - *Set the build directory to ProceduralMap-build/*
   - *Run CMake and build project*

