#! \bin\bash

g++ -O3 -o general main.cpp -mavx2 -msse4

for i in {1..10}
do
./general '/home/jqiu004/Desktop/Project/FSMbench/dot2table/benchmark_table/likeapple_495_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/clarissa/inputs/large_new.in' 495 256 8 8 0 1 1

./general '/home/jqiu004/Desktop/Project/FSMbench/dot2table/benchmark_table/likeapple_495_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/clarissa/inputs/large_new.in' 495 256 8 8 0 1 2

./general '/home/jqiu004/Desktop/Project/FSMbench/dot2table/benchmark_table/likeapple_495_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/clarissa/inputs/large_new.in' 495 256 8 8 0 1 3

./general '/home/jqiu004/Desktop/Project/FSMbench/dot2table/benchmark_table/likeapple_495_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/clarissa/inputs/large_new.in' 495 256 8 8 0 1 4

#./general '/home/jqiu004/Desktop/Project/FSMbench/dot2table/benchmark_table/likeapple_495_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/clarissa/inputs/large_new.in' 495 256 8 8 0 1 5
#./general '/home/jqiu004/Desktop/Project/FSMbench/dot2table/benchmark_table/likeapple_495_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/clarissa/inputs/large_new.in' 495 256 8 8 0 1 6
#./general '/home/jqiu004/Desktop/Project/FSMbench/dot2table/benchmark_table/likeapple_495_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/clarissa/inputs/large_new.in' 495 256 8 8 0 1 7

./general '/home/jqiu004/Desktop/Project/FSMbench/dot2table/benchmark_table/likeapple_495_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/clarissa/inputs/large_new.in' 495 256 8 8 0 1 8

./general '/home/jqiu004/Desktop/Project/FSMbench/dot2table/benchmark_table/likeapple_495_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/clarissa/inputs/large_new.in' 495 256 8 8 0 1 9

./general '/home/jqiu004/Desktop/Project/FSMbench/dot2table/benchmark_table/likeapple_495_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/clarissa/inputs/large_new.in' 495 256 8 8 0 1 10

./general '/home/jqiu004/Desktop/Project/FSMbench/dot2table/benchmark_table/likeapple_495_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/clarissa/inputs/large_new.in' 495 256 8 8 0 1 11

./general '/home/jqiu004/Desktop/Project/FSMbench/dot2table/benchmark_table/likeapple_495_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/clarissa/inputs/large_new.in' 495 256 8 8 0 1 13

./general '/home/jqiu004/Desktop/Project/FSMbench/dot2table/benchmark_table/likeapple_495_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/clarissa/inputs/large_new.in' 495 256 8 8 0 1 14

done
rm general
