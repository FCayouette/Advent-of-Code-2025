import sys

if __name__=="__main__":
    if len(sys.argv) >= 2:
        with open(sys.argv[1]) as input:
            part1 = 0
            part2 = 0
            pos = 50
            for line in input:
                delta = int(line[1:])
                part2 += delta // 100
                delta %= 100
                pos += -delta if line[0] == 'L' else delta
                if pos >= 100:
                    part2 += 1
                    pos -= 100
                elif pos < 0:
                    if pos != -delta:
                        part2 += 1
                    pos += 100
                elif pos == 0:
                    part2 += 1
                
                if pos == 0:
                    part1 += 1

            print(f"Part 1: {part1}\nPart 2: {part2}\n")    