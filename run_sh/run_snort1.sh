#! \bin\bash

g++ -O3 -o general main.cpp -mavx2 -msse4

for i in {1..10}
do
#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort1_86_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 86 256 8 8 0 1

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort1_86_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 86 256 8 8 0 2

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort1_86_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 86 256 8 8 0 3

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort1_86_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 86 256 8 8 0 4

./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort1_86_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 86 256 8 8 0 1 5
./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort1_86_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 86 256 8 8 0 1 6
./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort1_86_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 86 256 8 8 0 1 7

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort1_86_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 86 256 8 8 0 8

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort1_86_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 86 256 8 8 0 9

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort1_86_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 86 256 8 8 0 10

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/snort1_86_256_0.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/snort/huge_new.in' 86 256 8 8 0 11

done
rm general
