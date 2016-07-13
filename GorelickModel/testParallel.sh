#PBS -l nodes=1:ppn=1
#PBS -l walltime=48:00:00
#PBS -j oe
#PBS -o output/testParallel.out

cd $PBS_O_WORKDIR

time mpirun ./triangleSimulation -r 100 -t 1000 -s 1 -c 2 -b 3 -o 0
