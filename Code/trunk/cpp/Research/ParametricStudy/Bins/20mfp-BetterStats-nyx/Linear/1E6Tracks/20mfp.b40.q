#! /bin/sh
# Name of job
#PBS -N b40
# Resource list
#PBS -l nodes=1:ppn=1:opt2356
#PBS -l walltime=15:00:00
#PBS -l qos=preempt
# Is batch requeued if terminated prematurely
#PBS -r y

# Job working directory leave---blank for current directory
#PBS -d /home/jlconlin/Thesis/Code/trunk/cpp/Research/ParametricStudy
# Output file path
#PBS -o 20mfp.screen

# Define a queue
#PBS -q route

# email communication to:
#PBS -M jlconlin@umich.edu

# email options
#PBS -m abe

# Join output and error streams
#PBS -joe

# Export qsub environment varialbes to batch job
#PBS -V

#this is for helping you if a problem happens always include it
echo "I ran on: " $PBS_NODEFILE

#run job 
./../ArnoldiResearch --verbose 2 --width 20 --relaxed 0 --eigenvalues 3 --tracks 1000000 --inactive 50 --source Linear --Iterations 10 --active 500 --seed 0 --run --bins 40 -f Bins/20mfp-BetterStats-nyx/Linear/1E6Tracks/20mfp.b40.dat
    