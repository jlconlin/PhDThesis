#! /bin/sh
# Name of job
#PBS -N Test-94
# Resource list
#PBS -l nodes=1:ppn=1:opt2356
#PBS -l walltime=5:00:00
#PBS -l qos=preempt
#PBS -l pmem=1000mb
# Is batch requeued if terminated prematurely
#PBS -r y

# Job working directory leave---blank for current directory
#PBS -d /home/jlconlin/Thesis/Code/trunk/cpp/Research/Many
# Output file path
#PBS -o /home/jlconlin/Thesis/Code/trunk/cpp/Research/Many/Variance/50mfp/ArnoldiMany-94.screen

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
./../ArnoldiResearch --verbose 2 --width 50 --eigenvalues 3 --tracks 500000 --inactive 100 --source Histogram --Iterations 25 --active 100 --bins 75 -f /home/jlconlin/Thesis/Code/trunk/cpp/Research/Many/Variance/50mfp/ArnoldiMany-94.dat --run
        