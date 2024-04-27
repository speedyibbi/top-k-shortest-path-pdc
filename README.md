# Execution Instructions

## On Cluster

### Step 1: Compile the Code
```bash
mpic++ -fopenmp ./main.cpp -o a.out
```

### Step 2: Execute the Code

```bash
mpiexec -n 10 -f machinefile ./a.out

```
-  Replace machinefile with the path to your machine file containing the list of hosts.

## On WSL

### Step 1: Install MPI Implementation

Make sure you have MPI installed on your WSL. You can install it using the following command:

```bash
sudo apt-get install mpich
```

### Step 2: Compile the Code
```bash
mpic++ -fopenmp ./main.cpp -o a.out
```

### Step 3: Execute the Code

```bash
mpiexec -n 10 ./a.out
```




