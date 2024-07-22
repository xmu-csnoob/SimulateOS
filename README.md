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

## Compile and Run（General Method）

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

### 3. compile

```bash
cmake ..
make
```

### 4. run
```bash
cd ..
# run kernel
./kernel
# run testcases
./test
```

## Compile and Run（Recommended method）

### 1. (option 1, recommended) add this line to ~/.bashrc（Ubuntu）
```bash
source ~/your_work_path/SimulateOS/devtool/commands.sh
```

### 2. (option 2, only effective in current terminal) use the command in terminal

```bash
source ~/your_work_path/SimulateOS/devtool/commands.sh
```

### 3. 
```bash
cd SimulateOS
# mkk to build kernel
mkk
./kernel

# mkt to build test
mkt
./test

# mk to build both
mk
./kernel
./test

# mc to clear built executables
mc
```
