#!/bin/bash
#PBS -q dssc 
#PBS -l nodes=2:ppn=2 
#PBS -l walltime=0:10:00
cd $PBS_O_WORKDIR
make clean
module load openmpi-4.1.1+gnu-9.3.0
make
mpirun -np 2 --map-by node --report-bindings ./IMB-MPI1 PingPong -msglog 28 2>/dev/null | grep -v "#" |tr -s '[:blank:]' ',' | sed 's/^.//' | grep -v -e '^$' > infiniband_openmpi_node.csv
mpirun -np 2 --map-by socket --report-bindings ./IMB-MPI1 PingPong -msglog 28 2>/dev/null | grep -v "#" |tr -s '[:blank:]' ',' | sed 's/^.//' | grep -v -e '^$' > infiniband_openmpi_socket.csv
mpirun -np 2 --map-by core --report-bindings ./IMB-MPI1 PingPong -msglog 28 2>/dev/null | grep -v "#" |tr -s '[:blank:]' ',' | sed 's/^.//' | grep -v -e '^$' > infiniband_openmpi_core.csv
mpirun -np 2 --mca pml ob1 --mca btl tcp,self --map-by node --report-bindings ./IMB-MPI1 PingPong -msglog 28 2>/dev/null | grep -v "#" |tr -s '[:blank:]' ',' | sed 's/^.//' | grep -v -e '^$' > gigabit_openmpi_node.csv
mpirun -np 2 --mca pml ob1 --mca btl tcp,self --map-by socket --report-bindings ./IMB-MPI1 PingPong -msglog 28 2>/dev/null | grep -v "#" |tr -s '[:blank:]' ',' | sed 's/^.//' | grep -v -e '^$' > gigabit_openmpi_socket.csv
mpirun -np 2 --mca pml ob1 --mca btl tcp,self --map-by core --report-bindings ./IMB-MPI1 PingPong -msglog 28 2>/dev/null | grep -v "#" |tr -s '[:blank:]' ',' | sed 's/^.//' | grep -v -e '^$' > gigabit_openmpi_core.csv
module purge
make clean
module load intel
make
mpirun -np 2 -genv I_MPI_PIN_PROCESSOR_LIST 0,2 ./IMB-MPI1 PingPong -msglog 28 2>/dev/null | grep -v "#" |tr -s '[:blank:]' ',' | sed 's/^.//' | grep -v -e '^$' > infiniband_intel_node.csv
mpirun -np 2  -genv I_MPI_PIN_PROCESSOR_LIST 0,1 ./IMB-MPI1 PingPong -msglog 28 2>/dev/null | grep -v "#" |tr -s '[:blank:]' ',' | sed 's/^.//' | grep -v -e '^$' > infiniband_intel_socket.csv
mpirun -np 2 -f $PBS_NODEFILE ./IMB-MPI1 PingPong -msglog 28 2>/dev/null | grep -v "#" |tr -s '[:blank:]' ',' | sed 's/^.//' | grep -v -e '^$' > infiniband_intel_core.csv
exit 0