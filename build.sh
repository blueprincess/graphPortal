#!/bin/bash
mkdir -p bin
echo "Compiling the graph game, this will take some time"
g++ game/*.cpp boids/*.cpp stein/*.cpp stein/glew/*.c -I. -lGL -lSDL -lSDL_mixer -O3 -Wall -o bin/graph
echo "done ! Now go in the bin directory and launch ./graph"
