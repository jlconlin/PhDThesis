#! /bin/sh
# Name of job
#PBS -N Arnoldi0.2
# Resource list
#PBS -l nodes=1:ppn=1:opt2356
#PBS -l walltime=1:00:00
#PBS -l qos=preempt
#PBS -l pmem=1000mb
# Is batch requeued if terminated prematurely
#PBS -r y

# Job working directory leave---blank for current directory
#PBS -d /home/jlconlin/Thesis/Code/trunk/cpp
# Output file path
#PBS -o /home/jlconlin/Thesis/Code/trunk/cpp/ArnoldiBasic-w0.2.screen

# Define a queue
#PBS -q route

# email communication to:
#PBS -M jlconlin@umich.edu

# email options
#PBS -m abe

# Join output and error streams
#PBS -joe

# Export qsub environment variables to batch job
#PBS -V

#this is for helping you if a problem happens always include it
echo "I ran on: " $PBS_NODEFILE

#run job 
./Research/ArnoldiResearch -f ArnoldiBasic-w0.2.dat -t 100000 -i 25 -a 100 -w 0.2 -n 3 --run
        