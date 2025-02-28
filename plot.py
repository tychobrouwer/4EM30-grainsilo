import matplotlib.pyplot as plt

cell_sizes=[0.36, 0.1714, 0.090, 0.0400, 0.0225, 0.0144, 0.0100]
execution_times_omp=[4.177302284652756, 6.406663416829949, 8.586871562592773, 10.849768374924363, 11.17963160299917, 11.377280170454698, 10.806663209873841]
execution_times_no_omp=[1.769328538040396, 3.2008452649440895, 5.097430478585979, 7.565347934188546, 9.912282483200624, 12.239804593239496, 13.522199909692285]

# Plot execution times
plt.plot(cell_sizes, execution_times_omp, "-b", marker='o', label="omp")
plt.plot(cell_sizes, execution_times_no_omp, "-r", marker='o', label="no omp")
# plt.axhline(execution_org_alg, color="g", linestyle="-", label="orginal algorithm")
plt.legend()
plt.xlabel("cell size [m^2]", fontsize=16)
plt.ylabel("Speed-up", fontsize=16)
plt.title("Speed-up of the algorithm against cell size", fontsize=16)
plt.grid()
plt.show()
