import sys

def FindMax(enum, start, depthToGo):
    (i,m) = max(enum[start:] if depthToGo == 0 else enum[start:-depthToGo],key=lambda x : x[1])
    return m if depthToGo == 0 else m*10**depthToGo + FindMax(enum, i+1, depthToGo-1)

if __name__ == "__main__":
    part1, part2 = 0, 0
    if len(sys.argv) > 1:
        with open(sys.argv[1]) as inputFile:
            for line in inputFile:
                enum = list(enumerate([int(c) for c in line.strip()]))
                part1 += FindMax(enum, 0, 1)
                part2 += FindMax(enum, 0, 11)
    print(f"Part 1: {part1}\nPart 2: {part2}")