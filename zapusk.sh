rm out/*
rm result
g++ lab.cpp -o prog -g 2> errors
./prog 20 20 21 21 91 0.1
gnuplot result
