#! \bin\bash

g++ -O3 -o general main.cpp -mavx2 -msse4

for i in {1..1}
do
./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/div7_7_2_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/div/large.in' 7 2 8 8 0 0 1

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/div7_7_2_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/div/large.in' 7 2 8 8 0 0 2

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/div7_7_2_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/div/large.in' 7 2 8 8 0 0 3

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/div7_7_2_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/div/large.in' 7 2 8 8 0 0 4

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/div7_7_2_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/div/large.in' 7 2 8 8 0 0 5

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/div7_7_2_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/div/large.in' 7 2 8 8 0 0 8

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/div7_7_2_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/div/large.in' 7 2 8 8 0 0 9

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/div7_7_2_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/div/large.in' 7 2 8 8 0 0 10

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/div7_7_2_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/div/large.in' 7 2 8 8 0 0 11

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/div7_7_2_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/div/large.in' 7 2 8 8 0 0 13

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/div7_7_2_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/div/large.in' 7 2 8 8 0 0 14

done

rm general
