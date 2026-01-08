import sys

if __name__ == "__main__":
    if len(sys.argv) > 1:
        with open(sys.argv[1]) as inputStream:
            lines = [x.strip() for x in inputStream if 'S' in x or '^' in x]
            part1 = 0
            cur = {lines[0].index('S'): 1}
            for l in lines[1:]:
                next = {}
                for k,v in cur.items():
                    if l[k] == '^':
                        next[k-1] = next.get(k-1, 0) + v
                        next[k+1] = next.get(k+1, 0) + v
                        part1 += 1
                    else:
                        next[k] = next.get(k, 0) + v
                cur = next
            
            print(f"Part 1: {part1}\nPart 2: {sum(cur.values())}")