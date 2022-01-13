## Section 1

To compile the programs in this section 

```bash
module load openmpi-4.1.1+gnu-9.3.0
mpiCC -o ring.x ring.cpp
mpicc -o sum3Dmatrix.x sum3Dmatrix.c
```

To run the bidirectional ring

```bash
module load openmpi-4.1.1+gnu-9.3.0
mpirun -np [NUMBER OF PROCESSORS] ring.x [NUMBER OF ITERATIONS]
```

To run the parallel sum program on a `([x_dim],[y_dim],[z_dim])` tensor

```bash
module load openmpi-4.1.1+gnu-9.3.0
mpirun -np [NUMBER OF PROCESSORS] sum3Dmatrix.x [x_dim] [y_dim] [z_dim] [x_proc] [y_proc] [z_proc] [NUMBER OF ITERATIONS]
```
