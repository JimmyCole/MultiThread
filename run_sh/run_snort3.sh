#! \bin\bash

g++ -O3 -o general main.cpp -mavx2 -msse4

for i in {1..10}
do
#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort3_15_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 15 256 8 8 0 1 1

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort3_15_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 15 256 8 8 0 1 2

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort3_15_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 15 256 8 8 0 1 3

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort3_15_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 15 256 8 8 0 1 4

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort3_15_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 15 256 8 8 0 1 5

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort3_15_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 15 256 8 8 0 1 8

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort3_15_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 15 256 8 8 0 1 9

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort3_15_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 15 256 8 8 0 1 10

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort3_15_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 15 256 8 8 0 1 11

./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort3_15_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 15 256 8 8 0 1 13
./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort3_15_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 15 256 8 8 0 1 14

done
rm general
