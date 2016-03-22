#! /bin/sh
# Name of job
#PBS -N Power20
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
#PBS -o /home/jlconlin/Thesis/Code/trunk/cpp/PowerBasic-w20.screen

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
./Research/PowerResearch -f PowerBasic-w20.dat -t 100000 -i 250 -a 1000 -w 20.0 -b 75 --terminal dumb --run
        
