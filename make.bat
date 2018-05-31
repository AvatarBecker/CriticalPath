g++ -c  -std=c++11 -static-libstdc++ tTask.cpp
g++ -c  -std=c++11 -static-libstdc++ main.cpp
g++ -o test_files/CriticalPathFinder tTask.o main.o
rm -rf *.o