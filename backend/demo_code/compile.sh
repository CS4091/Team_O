# make script executable via "chmod +x compile.sh"

g++ -std=c++11 -O3 -o RouterDemo RouterDemo.cpp ../src/aircraft.cpp ../src/router1.cpp ../src/map.cpp
g++ -std=c++11 -O3 -o FloodFillDemo FloodFillDemo.cpp ../src/aircraft.cpp ../src/router1.cpp ../src/map.cpp
