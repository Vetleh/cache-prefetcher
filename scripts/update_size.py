import sys

path = './src'


# updates size parameter in file
def update_size(size):
    file = open(f'{path}/prefetcher.cc', 'r')
    lines = file.readlines()

    for (index, line) in enumerate(lines):
        if line[:12] == '#define SIZE':
            lines[index] = f'{line[:12]} = {size};\n'

    file.close()

    file = open(f'{path}/prefetcher.cc', 'w')
    for line in lines:
        file.write(line)
    file.close()


if __name__ == "__main__":
    update_size(sys.argv[1])
