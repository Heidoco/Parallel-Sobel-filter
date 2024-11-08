# Compilar

g++ -std=c++11 -fopenmp -o edge_opemp edge_opemp.cpp $(pkg-config --cflags --libs opencv4)

g++ -std=c++11 -o edge_threads edge_threads.cpp $(pkg-config --cflags --libs opencv4)
