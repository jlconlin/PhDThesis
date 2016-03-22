#! /bin/sh
# Name of job
#PBS -N I35
# Resource list
#PBS -l nodes=1:ppn=1:opt2356
#PBS -l walltime=5:00:00
#PBS -l qos=preempt
#PBS -l pmem=1000mb
# Is batch requeued if terminated prematurely
#PBS -r y

# Job working directory leave---blank for current directory
#PBS -d /home/jlconlin/Thesis/Code/trunk/cpp/Research/ParametricStudy
# Output file path
#PBS -o /home/jlconlin/Thesis/Code/trunk/cpp/Research/ParametricStudy/Multimedia/Iterations/ArnoldiMulti.I35.screen

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
./../ArnoldiResearch --verbose 2 --width 20 --relaxed 0 --multimedia --eigenvalues 2 --tracks 500000 --inactive 25 --source Histogram --active 100 --seed 0 --bins 300 --run --Iterations 35 -f Multimedia/Iterations/ArnoldiMulti.I35.dat
        