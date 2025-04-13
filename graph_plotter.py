import matplotlib.pyplot as plt

# Read performance data
with open("performance_data.txt", "r") as file:
    data = file.readlines()

labels = []
times = []

for line in data:
    parts = line.strip().split()
    labels.append(parts[0] + " " + parts[1])  # "Without Threads" or "With Threads"
    times.append(float(parts[2]))  # Execution time in ms

# Plot the graph
plt.bar(labels, times, color=['red', 'green'])
plt.ylabel("Execution Time (ms)")
plt.title("Performance Comparison: Without Threads vs. With Threads")
plt.show()
