#! /bin/sh
# Name of job
#PBS -N SymmetricArnoldi
# Resource list
#PBS -l nodes=1:ppn=1:opt2356
#PBS -l walltime=10:00:00
#PBS -l qos=preempt
#PBS -l pmem=1000mb
# Is batch requeued if terminated prematurely
#PBS -r y

# Job working directory leave---blank for current directory
#PBS -d /home/jlconlin/Thesis/Code/trunk/cpp/
# Output file path
#PBS -o /home/jlconlin/Thesis/Code/trunk/cpp/Symmetric.Arnoldi.screen

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
./Research/ArnoldiResearch --verbose 2 --width 20 --relaxed 0 --multimedia --eigenvalues 2 --inactive 25 --source Histogram --Iterations 10 --active 225 --seed 0 --bins 300 --run --tracks 1000000 -f Symmetric.Arnoldi.dat
        
