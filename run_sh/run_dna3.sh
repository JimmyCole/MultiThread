#! \bin\bash

g++ -O3 -pthread -mavx2 -msse4 -o general  '/home/jqiu004/Desktop/Project/Project_Apr./Project_20160403/Multithread/NoOutput/main.cpp' 

for i in {1..10}
do
#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna3_40_4_16_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 40 4 8 8 0 2 1

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna3_40_4_16_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 40 4 8 8 0 2 2

#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna3_40_4_16_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 40 4 8 8 0 2 3

#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna3_40_4_16_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 40 4 8 8 0 2 4

#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna3_40_4_16_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 40 4 8 8 0 2 5
#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna3_40_4_16_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 40 4 8 8 0 2 6
#./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna3_40_4_16_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 40 4 8 8 0 2 7

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna3_40_4_16_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 40 4 8 8 0 2 8

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna3_40_4_16_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 40 4 8 8 0 2 9

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna3_40_4_16_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 40 4 8 8 0 2 10

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna3_40_4_16_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 40 4 8 8 0 2 11

done

mkdir Record
cd Record
mkdir DNA3
cd ..
cp -a R*.txt Record/DNA3/
rm general
rm R*.txt

