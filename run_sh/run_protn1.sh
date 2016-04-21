#! \bin\bash

g++ -O3 -o general main.cpp -mavx2 -msse4

for i in {1..10}
do
#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn1_69_20_30_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 69 20 8 8 0 1

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn1_69_20_30_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 69 20 8 8 0 2

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn1_69_20_30_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 69 20 8 8 0 3

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn1_69_20_30_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 69 20 8 8 0 4

./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn1_69_20_30_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 69 20 8 8 0 3 5
./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn1_69_20_30_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 69 20 8 8 0 3 6
./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn1_69_20_30_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 69 20 8 8 0 3 7

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn1_69_20_30_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 69 20 8 8 0 8

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn1_69_20_30_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 69 20 8 8 0 9

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn1_69_20_30_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 69 20 8 8 0 10

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn1_69_20_30_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 69 20 8 8 0 11

done
rm general
