import sys
from scipy.optimize import linprog

if __name__ == "__main__":
    part2 = 0
    with open(sys.argv[1]) as inputFile:
        for line in inputFile:
            _, *f, j = line.split()
            flippers = [set(map(int, x[1:-1].split(','))) for x in f]
            joltage = tuple(map(int, j[1:-1].split(',')))
            part2 += int(linprog([1]*len(flippers), 
                                 A_eq=[[i in x for x in flippers] for i in range(0, len(joltage))],
                                 b_eq=joltage,
                                 integrality=True).fun)
    print(f"Part 2: {part2}")