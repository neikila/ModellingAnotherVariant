rm out/*
rm result
g++ lab.cpp -o prog -g 2> errors
./prog 10 15 21 31 10 0.1
gnuplot result
