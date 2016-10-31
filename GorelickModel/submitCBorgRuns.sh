#!/bin/bash

# How many nodes, processors, and hours to request for each seed?
NODES=2
PPN=2
WALLHOURS=48

# 6 formulations (0 - 5)
FORMULATIONS=$(seq 0 1)
SEEDS=$(seq 0 1)

# Objective Types: 0 (Max value among utilities) or 1 (average value across utilities)
OBJ_TYPE=0

for FORMULATION in ${FORMULATIONS}
do
	for SEED in ${SEEDS}
	do
		NAME=CBorg_NCTriangle_O${OBJ_TYPE}_F${FORMULATION}_S${SEED}
		echo "Submitting: ${NAME}"

		PBS="#!/bin/bash\n\
		#PBS -N ${NAME}\n\
		#PBS -l nodes=${NODES}:ppn=${PPN}\n\
		#PBS -l walltime=${WALLHOURS}:00:00\n\
		#PBS -o output/${NAME}.out\n\
		#PBS -e error/${NAME}.err\n\
		cd \$PBS_O_WORKDIR\n\
		mpirun ./triangleSimulation -r 100 -t 100 -o 0 -s ${SEED} -c ${FORMULATION} -b ${OBJ_TYPE}"

		echo -e ${PBS} | qsub 
		sleep 0.5
		echo "done."
	done
done

