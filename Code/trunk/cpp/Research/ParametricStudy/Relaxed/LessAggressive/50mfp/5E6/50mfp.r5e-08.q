#! /bin/sh
# Name of job
#PBS -N r5e-08
# Resource list
#PBS -l nodes=1:ppn=1:opt2356
#PBS -l walltime=30:00:00
#PBS -l qos=preempt
# Is batch requeued if terminated prematurely
#PBS -r y

# Job working directory leave---blank for current directory
#PBS -d /home/jlconlin/Thesis/Code/trunk/cpp/Research/ParametricStudy
# Output file path
#PBS -o /home/jlconlin/Thesis/Code/trunk/cpp/Research/ParametricStudy/Relaxed/LessAggressive/50mfp/5E6/50mfp.r5e-08.screen

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
./../ArnoldiResearch --verbose 2 --width 50 --eigenvalues 3 --tracks 5000000 --inactive 100 --source Histogram --Iterations 25 --active 100 --seed 0 --bins 75 --run --relaxed 5e-08 -f Relaxed/LessAggressive/50mfp/5E6/50mfp.r5e-08.dat
        
