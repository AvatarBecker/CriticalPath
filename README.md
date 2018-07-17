## Critical Path Finder (Project Management)

This piece of software was written as part of the requirements of a project management course at UFPR 
(Federal University of Parana).

It takes an input file with a fixed name in the same folder as the executable, containing the description of
the tasks and how they connect describe in a specific syntax (an example file is in the folder test_files).
The program processes the file, and outputs a list of all the possible paths, together with the identified 
critical path(s).

Right now, it is a mish-mash of portuguese and english, but I think I will keep it that way since it can 
be of help to the students taking that same course at the Universty.

### Compiling

Using g++, compile it like so:

```bat
g++ -c  -std=c++11 -static-libstdc++ tTask.cpp
g++ -c  -std=c++11 -static-libstdc++ main.cpp
g++ -o CriticalPathFinder tTask.o main.o
```

In Windows, just run the make.bat. It will output the executable to the test_files folder, where a test file
awaits.

### Running It

In the test_files folder, just run the executable.

In Windows, sometimes you will get an **error related to libstdc++-6.dll**, despite linking the libraries 
statically as we did. In that case, **copying libstdc++-6.dll from C:\MinGW\bin to the same folder as the 
executable does the trick**.

