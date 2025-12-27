import sys

if __name__ == "__main__":
    if len(sys.argv) > 1:
        part1, part2 = 0, 0
        ranges, merged = [], []
        with open(sys.argv[1]) as inputFile:
            numbers = False
            for line in [x.strip() for x in inputFile]:
                if len(line) == 0:
                    numbers = True
                    ranges = sorted(ranges)
                    merged.append(ranges[0])
                    for s,e in ranges[1:]:
                        ms, me = merged[-1]
                        if s <= me:
                            merged[-1] = (ms, max(e, me))
                        else:
                            merged.append((s,e))
                    continue
                if not numbers:
                    pos = line.find('-')
                    ranges.append((int(line[:pos]),int(line[pos+1:])))
                else:
                    fresh = False
                    val = int(line)
                    for s,e in merged:
                        if not (val < s or val > e):
                            fresh = True
                            break
                    part1 += fresh

        for s,e in merged:
            part2 += e-s + 1

        print (f"Part 1: {part1}\nPart 2: {part2}")

                    