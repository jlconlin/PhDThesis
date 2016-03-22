#! /bin/sh
# Name of job
#PBS -N ArnoldiVariance-0
# Resource list
#PBS -l nodes=1:ppn=1:opt2356
#PBS -l walltime=30:00:00
#PBS -l qos=preempt
#PBS -l pmem=1000mb
# Is batch requeued if terminated prematurely
#PBS -r y

# Job working directory leave---blank for current directory
#PBS -d /home/jlconlin/Thesis/Code/trunk/cpp/Research/Many
# Output file path
#PBS -o /home/jlconlin/Thesis/Code/trunk/cpp/Research/Many/Variance/100mfp/Arnoldi-0.screen

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
./../ArnoldiResearch -f /home/jlconlin/Thesis/Code/trunk/cpp/Research/Many/Variance/100mfp/Arnoldi-0.dat -w 100 -b 100 -n 3 -i 20 -a 100 -t 1000000 -I 25 --run
        