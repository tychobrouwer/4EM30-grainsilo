import re
import subprocess
import time
import matplotlib.pyplot as plt

# Path to the C header file
header_file = "mylib.h"
consts_file = "consts.h"

# Values to update in the format [(NR_CELL_X, NR_CELL_Y), ...]
values = [(5, 10), (7, 15), (10, 20), (15, 30), (20, 40), (30, 60)]

execution_times_omp = []
execution_times_no_omp = []

def update_header(file_path, x_val, y_val):
    with open(file_path, "r") as file:
        content = file.read()
    
    # Update NR_CELL_X
    content = re.sub(r"#define NR_CELL_X \d+", f"#define NR_CELL_X {x_val}", content)
    # Update NR_CELL_Y
    content = re.sub(r"#define NR_CELL_Y \d+", f"#define NR_CELL_Y {y_val}", content)
    
    with open(file_path, "w") as file:
        file.write(content)

def update_omp(file_path, enable):
    with open(file_path, "r") as file:
        content = file.read()

    value = 1 if enable else 0
    content = re.sub(r"#define ENABLE_OMP \d+", f"#define ENABLE_OMP {value}", content)

    with open(file_path, "w") as file:
        file.write(content)

def update_alg(file_path, enable):
    with open(file_path, "r") as file:
        content = file.read()

    value = 1 if enable else 0
    content= re.sub(r"#define ENABLE_LL_ALG \d+", f"#define ENABLE_LL_ALG {value}", content)

    with open(file_path, "w") as file:
        file.write(content)

def run():
    # Run make clean && make
    subprocess.run(["make", "clean"], check=True)
    subprocess.run(["make"], check=True)
    
    # Run the binary
    subprocess.run(["./silo"], check=True)

# Loop through values and update the header file
update_omp(consts_file, True)
update_alg(consts_file, True)
for x, y in values:
    update_header(header_file, x, y)

    start_time = time.time()
    run()
    end_time = time.time()

    execution_time = end_time - start_time
    execution_times_omp.append(execution_time)

update_omp(consts_file, False)
for x, y in values:
    update_header(header_file, x, y)

    start_time = time.time()
    run()
    end_time = time.time()

    execution_time = end_time - start_time
    execution_times_no_omp.append(execution_time)

update_alg(consts_file, False)

start_time = time.time()
run()
end_time = time.time()

execution_org_alg = end_time - start_time

# Plot execution times
plt.plot([f"({x}, {y})" for x, y in values], execution_times_omp, "-b", marker='o', label="omp")
plt.plot([f"({x}, {y})" for x, y in values], execution_times_no_omp, "-r", marker='o', label="no omp")
plt.axhline(execution_org_alg, color="g", linestyle="-", label="orginal algorithm")
plt.legend()
plt.xlabel("NR_CELL_X, NR_CELL_Y")
plt.ylabel("Execution Time (seconds)")
plt.title("Execution Time vs. Grid Size")
plt.grid()
plt.show()
plt.savefig("results.png")

