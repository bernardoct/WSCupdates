#PBS -l nodes=1:ppn=10
#PBS -l walltime=48:00:00
#PBS -j oe
#PBS -o output/testParallel.out

cd $PBS_O_WORKDIR

time mpirun ./triangleSimulation -r 1000 -t 1000 -s 0 -c 0 -b 3 -o 0
