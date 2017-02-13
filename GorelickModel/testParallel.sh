#PBS -l nodes=9:ppn=15
#PBS -l walltime=24:00:00
#PBS -j oe
#PBS -o output/testParallel.out

cd $PBS_O_WORKDIR

time mpirun ./triangleSimulation -r 1000 -t 1000 -s 0 -c 2 -b 3 -o 0
