class Benchmark:
    def __init__(self, name, ipc, speedup, accuracy, coverage):
        self.name = name
        self.ipc = ipc
        self.speedup = speedup
        self.accuracy = accuracy
        self.coverage = coverage

    # prints benchmark
    def print_benchmark(self):
        print(
            f'{self.name:10} \t {self.ipc} \t {self.speedup} \t {self.accuracy} \t {self.coverage}')


class Statistics:
    def __init__(self, data, size):
        self.size = size
        self.benchmarks = []

        # overall speedup
        self.speedup = data[-2].split()[1]

        # creates benchmark objects
        benchmarks = data[185:197]
        for benchmark in benchmarks:
            p = benchmark.split()
            self.benchmarks.append(Benchmark(p[0], p[1], p[2], p[3], p[4]))

    # prints stats
    def print_stats(self):
        print(f'Size: {self.size}')
        print(f'Speedup: {self.speedup}\n')
        for benchmark in self.benchmarks:
            benchmark.print_benchmark()


size = 256

# reads stats from file
file = open('./stats.txt', 'r')
data = file.readlines()
stats = Statistics(data, size)

stats.plot_speedup()


# # reads the overall performance speedup
# speedup = stats[-2].split()[1]

# # reads the benchmark results
# benchmarks = stats[185:197]

# # closes file
# file.close()
