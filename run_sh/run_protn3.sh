#! \bin\bash

g++ -O3 -o general main.cpp -mavx2 -msse4

for i in {1..10}
do
#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn3_832_20_615_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 832 20 8 8 0 3 1

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn3_832_20_615_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 832 20 8 8 0 3 2

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn3_832_20_615_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 832 20 8 8 0 3 3

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn3_832_20_615_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 832 20 8 8 0 3 4

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn3_832_20_615_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 832 20 8 8 0 3 5
#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn3_832_20_615_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 832 20 8 8 0 3 6
#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn3_832_20_615_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 832 20 8 8 0 3 7

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn3_832_20_615_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 832 20 8 8 0 3 8

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn3_832_20_615_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 832 20 8 8 0 3 9

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn3_832_20_615_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 832 20 8 8 0 3 10

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn3_832_20_615_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 832 20 8 8 0 3 11

./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn3_832_20_615_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 832 20 8 8 0 3 13

./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn3_832_20_615_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 832 20 8 8 0 3 14


done
rm general
