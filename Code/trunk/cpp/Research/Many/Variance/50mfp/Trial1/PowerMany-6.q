#! /bin/sh
# Name of job
#PBS -N Test-6
# Resource list
#PBS -l nodes=1:ppn=1:opt2356
#PBS -l walltime=5:00:00
#PBS -l qos=preempt
#PBS -l pmem=3000mb
# Is batch requeued if terminated prematurely
#PBS -r y

# Job working directory leave---blank for current directory
#PBS -d /home/jlconlin/Thesis/Code/trunk/cpp/Research/Many
# Output file path
#PBS -o /home/jlconlin/Thesis/Code/trunk/cpp/Research/Many/Variance/50mfp/PowerMany-6.screen

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
./../PowerResearch --verbose 2 --width 50  --tracks 500000 --inactive 2500   --active 2500 --bins 75 -f /home/jlconlin/Thesis/Code/trunk/cpp/Research/Many/Variance/50mfp/PowerMany-6.dat --run
        
