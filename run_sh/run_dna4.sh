#! \bin\bash

g++ -O3 -o general main.cpp -mavx2 -msse4

for i in {1..10}
do

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna4_34_4_7_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 34 4 8 8 0 2 1

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna4_34_4_7_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 34 4 8 8 0 2 2

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna4_34_4_7_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 34 4 8 8 0 2 3

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna4_34_4_7_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 34 4 8 8 0 2 4

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna4_34_4_7_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 34 4 8 8 0 2 5

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna4_34_4_7_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 34 4 8 8 0 2 6

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna4_34_4_7_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 34 4 8 8 0 2 7

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna4_34_4_7_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 34 4 8 8 0 2 8

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna4_34_4_7_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 34 4 8 8 0 2 9

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna4_34_4_7_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 34 4 8 8 0 2 10

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna4_34_4_7_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 34 4 8 8 0 2 11

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna4_34_4_7_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 34 4 8 8 0 2 13

./general '/home/jqiu004/Desktop/Project/Project_Feb./Project_20160226/benchmark_table/dna4_34_4_7_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/dna/large.in' 34 4 8 8 0 2 14

done
rm general
