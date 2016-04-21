#! \bin\bash

g++ -O3 -pthread -mavx2 -msse4 -o general  '/home/jqiu004/Desktop/Project/Project_Apr./Project_20160403/Multithread/NoOutput/main.cpp' 

for i in {1..10}
do
#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn2_281_20_254_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 281 20 8 8 0 3 1

./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn2_281_20_254_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 281 20 8 8 0 3 2

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn2_281_20_254_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 281 20 8 8 0 3 3

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn2_281_20_254_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 281 20 8 8 0 3 4

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn2_281_20_254_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 281 20 8 8 0 3 5

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn2_281_20_254_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 281 20 8 8 0 3 6

#./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn2_281_20_254_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 281 20 8 8 0 3 7

./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn2_281_20_254_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 281 20 8 8 0 3 8

./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn2_281_20_254_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 281 20 8 8 0 3 9

./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn2_281_20_254_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 281 20 8 8 0 3 10

./general '/home/jqiu004/Desktop/Project/Project_Mar./Project_20160306/Benchmark_Table/protn2_281_20_254_new.table' '/home/jqiu004/Desktop/Project/FSMbench/Input_modify/protn/large.in' 281 20 8 8 0 3 11

done
rm general

mkdir Record
cd Record
mkdir PROTN2
cd ..
cp -a R*.txt Record/PROTN2/
rm general
rm R*.txt
