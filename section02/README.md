## Section 2

The last release for the Intel MPI Benchmark can be downloaded and compiled with

```bash
wget https://github.com/intel/mpi-benchmarks/archive/refs/tags/IMB-v2021.3.zip
unzip IMB-v2021.3.zip
cd mpi-benchmarks-IMB-v2021.3 
cd src_c
make
```

To execute various tests, see the script `PingPong.sh`.

```bash
qsub PingPong.sh
```