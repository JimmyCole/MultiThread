#! \bin\bash

g++ -O3 -o general main.cpp -mavx2 -msse4

for i in {1..10}
do

#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/evenodd_4_4_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/evenodd/inputs/large.in' 4 4 8 8 0 4 1

#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/evenodd_4_4_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/evenodd/inputs/large.in' 4 4 8 8 0 4 2

#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/evenodd_4_4_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/evenodd/inputs/large.in' 4 4 8 8 0 4 3

#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/evenodd_4_4_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/evenodd/inputs/large.in' 4 4 8 8 0 4 4

#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/evenodd_4_4_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/evenodd/inputs/large.in' 4 4 8 8 0 4 5
#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/evenodd_4_4_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/evenodd/inputs/large.in' 4 4 8 8 0 4 6
#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/evenodd_4_4_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/evenodd/inputs/large.in' 4 4 8 8 0 4 7

#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/evenodd_4_4_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/evenodd/inputs/large.in' 4 4 8 8 0 4 8

#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/evenodd_4_4_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/evenodd/inputs/large.in' 4 4 8 8 0 4 9

#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/evenodd_4_4_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/evenodd/inputs/large.in' 4 4 8 8 0 4 10

#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/evenodd_4_4_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/evenodd/inputs/large.in' 4 4 8 8 0 4 11

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/evenodd_4_4_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/evenodd/inputs/large.in' 4 4 8 8 0 4 13

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/evenodd_4_4_0_new.table' '/home/jqiu004/Desktop/Project/FSMbench/FSMbench/evenodd/inputs/large.in' 4 4 8 8 0 4 14

done
rm general
