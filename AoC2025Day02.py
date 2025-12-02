import sys, re

part1Matcher = re.compile(r"""^(\d+)\1$""")
part2Matcher = re.compile(r"""^(\d+)\1+$""")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        part1, part2 = 0, 0
        with open(sys.argv[1]) as inputFile:
            for line in inputFile:
                for sRange in line.split(','):
                    vals = [int(x) for x in sRange.split("-")]
                    for x in range(vals[0], vals[1]+1):
                        sx = str(x)
                        if part1Matcher.match(sx):
                            part1 += x
                            part2 += x
                        elif part2Matcher.match(sx):
                            part2 += x
            print(f"Part 1: {part1}\nPart 2: {part2}")   