#! /bin/sh
# Name of job
#PBS -N Test
# Resource list
#PBS -l nodes=1:ppn=1:opt2356
#PBS -l walltime=3:00:00
#PBS -l qos=preempt
#PBS -l pmem=3000mb
# Is batch requeued if terminated prematurely
#PBS -r y

# Job working directory leave---blank for current directory
#PBS -d /home/jlconlin/Thesis/Code/trunk/cpp
# Output file path
#PBS -o /home/jlconlin/Thesis/Code/trunk/cpp/50mfp.Power.screen

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
./Research/PowerResearch -w 50 -f 50mfp.Power.dat -t 500000 -i 2500 -a 2500 -b 75 --run
        