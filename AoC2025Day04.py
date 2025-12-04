import sys

if __name__=="__main__":
    if len(sys.argv) > 1:
        map = []
        lineX = 0
        rolls, next = set(), set()
        with open(sys.argv[1]) as inputFile:
            for line in inputFile:
                for x in range(0, len(line)):
                    if line[x] == '@':
                        rolls.add((lineX, x))
                lineX += 1

        directions = [(-1,-1), (-1,0), (-1,1), (0,-1), (0,1), (1,-1), (1,0), (1,1)]
        goOn = True
        part1, part2, round = 0,0,0
        while goOn:
            goOn = False
            for x,y in rolls:
                count = 0
                for dx,dy in directions:
                    nx, ny = x+dx,y+dy
                    if (nx,ny) in rolls:
                        count += 1
                if count < 4:       
                    goOn = True
                    part2 += 1
                    if round == 0:
                        part1 += 1
                else:
                    next.add((x,y))
            
            round += 1
            rolls = rolls.intersection(next)
            next.clear()
            
        print(f"Part 1: {part1}\nPart 2: {part2}")