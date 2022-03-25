import os
import sys

parameters = [
    # GHB   IT   W  H  D   STRIDED
#    [256, 256, 1, 1, 1 , 'true'], 
#    [256, 256, 2, 1, 2 , 'true'], 
#    [256, 256, 4, 1, 4 , 'true'], 
#    [256, 256, 8, 1, 8 , 'true'], 
#    [256, 256, 16, 1, 16 , 'true'], 
#    [256, 256, 1, 2, 2 , 'true'], 
#    [256, 256, 1, 4, 4 , 'true'], 
    [256, 256, 1, 8, 8 , 'true'], 
    [256, 256, 1, 16, 16 , 'true'], 
    [256, 256, 2, 2, 2 , 'true'], 
    [256, 256, 4, 4, 4 , 'true'], 
    [256, 256, 8, 8, 8 , 'true'], 
    [256, 256, 16, 16, 16 , 'true'], 
    [16, 16, 4, 1, 4 , 'true'], 
    [16, 16, 1, 4, 4 , 'true'], 
    [16, 16, 4, 4, 4 , 'true'], 
    [8, 8, 4, 1, 4 , 'true'], 
    [8, 8, 1, 4, 4 , 'true'], 
    [8, 8, 4, 4, 4 , 'true'], 
    [4, 4, 4, 1, 4 , 'true'], 
    [4, 4, 1, 4, 4 , 'true'], 
    [4, 4, 4, 4, 4 , 'true'], 
    [2, 2, 4, 1, 4 , 'true'], 
    [2, 2, 1, 4, 4 , 'true'], 
    [2, 2, 4, 4, 4 , 'true'], 
]

def update_params(params):
    file = open('src/prefetcher.cc', 'r')
    lines = file.readlines()

    for (idx, line) in enumerate(lines):
        if '#define GHB_SIZE' in line: 
            lines[idx] = f'#define GHB_SIZE {params[0]}\n'
        elif '#define IT_SIZE' in line: 
            lines[idx] = f'#define IT_SIZE {params[1]}\n'
        elif '#define WIDTH' in line: 
            lines[idx] = f'#define WIDTH {params[2]}\n'
        elif '#define DEPTH' in line: 
            lines[idx] = f'#define DEPTH {params[3]}\n'
        elif '#define DEGREE' in line: 
            lines[idx] = f'#define DEGREE {params[4]}\n'
        elif '#define strided' in line: 
            lines[idx] = f'#define strided {params[5]}\n'

    file.close()

    file = open('src/prefetcher.cc', 'w')
    for line in lines:
        file.write(line)
    file.close()

if __name__ == "__main__":
    for params in parameters: 

        # updates the parameters 
        update_params(params)

        # compiles and tests the code 
        os.system('make compile test')

        # moves the stats 
        os.system(f'mv stats.txt test-output/g{params[0]}_i{params[1]}_w{params[2]}_h{params[3]}_d{params[4]}_s{params[5]}')

