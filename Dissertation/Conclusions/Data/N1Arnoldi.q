#! /bin/sh
# Name of job
#PBS -N Arnoldi20
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
#PBS -o /home/jlconlin/Thesis/Code/trunk/cpp/N1Arnoldi.screen

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
./Research/ArnoldiResearch -f N1Arnoldi.dat -t 100000 -i 125 -a 500 -I 2 -w 20.0 -n 1 -b 75 --terminal dumb --run
        