import sys, math

if __name__ == "__main__":
    if len(sys.argv) > 1:
        with open(sys.argv[1]) as inputFile:
            lines = inputFile.readlines()
            data = lines[:-1]
            ops = [x for x in lines[-1] if x == '+' or x =='*']
            partVals = []
            for line in data:
                nums = []
                val = 0
                for c in line.strip():
                    if c == ' ':
                        if val != 0:
                            nums.append(val)
                        val = 0
                    else:
                        val = val*10 + int(c)
                if val != 0:
                    nums.append(val)
                partVals.append(nums)
            
            part1, part2 = 0, 0
            for z in list(zip(*partVals, ops)):
                part1 += sum(z[:-1]) if z[-1] == '+' else math.prod(z[:-1])
        
            partVals.clear()
            zipped = list(zip(*data))
            for i in reversed(range(len(zipped)-1)):
                v = ''.join(zipped[i]).strip()
                if len(v):
                    partVals.append(int(v))
                if lines[-1][i] != ' ':
                    part2 += sum(partVals) if lines[-1][i] == '+' else math.prod(partVals)
                    partVals.clear()

            print(f"Part 1: {part1}\nPart 2: {part2}")
            