# SimulateOS

Simulate OS is a practice project on Operating System. It tried to simulate a real OS kernel, which features memory control, disk management, process management. Also it simulate the important computer components such as CPU.

## Components

### 1. src/hardwares

contains all the simulated hardwares, such as CPU and memory.

### 2. src/memory

provide memory control

### 3. src/process

provide process management

### 4. src/fs

provide a simple file system

### 5. src/disk

provide virtual disks and io operations.

## Updates

2024/7/18: first time of README update, now the kernel has some simple memory control and disk simulation, however still far from running a stored program.


## Compile and Run

### 1. create /build directory

```bash
cd SimulateOS
mkdir build
cd build
```

### 2. (optional) install cmake (for example on Ubuntu)

```bash
sudo apt-get update
sudo apt-get install cmake
```