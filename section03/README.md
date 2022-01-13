## Section 3

To compile, load the MPI module and then:

```bash
 mpif77 -ffixed-line-length-none Jacobi_MPI_vectormode.F -o jacobi3D.x
```

To run the code
```bash
mpirun --mca btl ^openib -np 24 ./jacoby3D.x 
```

Run the program with 4 processes across cores:
```bash
mpirun --mca btl ^openib -map-by core -np 4 jacobi3D.x < input.1200
```

Run the program with 4 processes across sockets:
```bash
mpirun --mca btl ^openib -map-by socket -np 4 jacobi3D.x < input.1200
```

Run the program with 12 processes across nodes:
```bash
mpirun --mca btl ^openib -map-by node -np 12 jacobi3D.x < input.1200
```
